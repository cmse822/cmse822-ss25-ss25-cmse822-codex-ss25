#include <iostream>
#include <mpi.h>
#include <vector>
#include <complex>

double f(double t){
    return 1 + 2*t;
}

static void fft1D(std::complex<double> *data, int N, bool inverse)
{
    int direction = inverse ? -1 : +1;

    // Bit-reversal permutation.
    int j = 0;
    for (int i = 0; i < N - 1; i++)
    {
        if (i < j)
        {
            std::swap(data[i], data[j]);
        }
        int m = N >> 1;
        while (j >= m && m > 0)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    // Cooley–Tukey iterative FFT.
    for (int step = 1; step < N; step <<= 1)
    {
        int step2 = step << 1;
        double theta = (direction * M_PI) / static_cast<double>(step);
        std::complex<double> wstep(std::cos(theta), std::sin(theta));
        for (int k = 0; k < N; k += step2)
        {
            std::complex<double> w(1.0, 0.0);
            for (int m = 0; m < step; m++)
            {
                std::complex<double> t = w * data[k + m + step];
                std::complex<double> u = data[k + m];
                data[k + m] = u + t;
                data[k + m + step] = u - t;
                w *= wstep;
            }
        }
    }

    // Normalize if performing an inverse FFT.
    if (inverse)
    {
        double invN = 1.0 / static_cast<double>(N);
        for (int i = 0; i < N; i++)
        {
            data[i] *= invN;
        }
    }
}

int main() {
    MPI_Comm comm = MPI_COMM_WORLD;
    int nprocs, procno;
    
    MPI_Init(0,0);
  
    // compute communicator rank and size
    MPI_Comm_size(comm,&nprocs);
    MPI_Comm_rank(comm,&procno);

    int sample_points = 16;
    int n_of_coefficients = 16;
    const double PI = M_PI;

    std::vector<std::complex<double>> coefficients(n_of_coefficients, 0);

    // Range for all processes
    double lower_range = 0;
    double end_range = 1;
    double total_interval = end_range - lower_range;
    // Step between each data point
    double step = total_interval / sample_points;

    // Range for local calculation
    int local_sample_points = sample_points / nprocs;
    int local_sample_start = procno * local_sample_points;
    int local_sample_end = local_sample_start + local_sample_points;

    for(int n = 0; n < n_of_coefficients; n++){
        // Calculate the local sum
        std::complex<double> local_sum(0.0,0.0);
        for(int p = local_sample_start; p < local_sample_end; p++){
            double f_i = f(p*step);
            double phase = 2 * PI * n * p * step;
            auto e = std::complex<double>(std::cos(phase),std::sin(phase));
            local_sum += f_i * e;
        }
        std::complex<double> global_sum;
        MPI_Allreduce(&local_sum, &global_sum, 1, MPI_CXX_DOUBLE_COMPLEX, MPI_SUM, comm);
        coefficients.at(n) = global_sum;
    }
  
    if(procno == 0){
        std::vector<std::complex<double>> points;
        for(double s = lower_range; s <= end_range; s += step){
            points.emplace_back(f(s), 0);
        }

        fft1D(points.data(), sample_points, false);

        std::cout << "Provided Cooley Tookey implementation" << std::endl;

        for(int n = 0; n < n_of_coefficients; n++){
            std::cout << points.at(n) << ", ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "Discrete implementation" << std::endl;
        for(int n = 0; n < n_of_coefficients; n++){
            std::cout << coefficients.at(n) << ", ";
        }
        std::cout << std::endl;
    }
    MPI_Finalize();
    return 0;
  }