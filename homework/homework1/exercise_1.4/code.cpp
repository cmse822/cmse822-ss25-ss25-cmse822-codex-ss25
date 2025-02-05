#include <iostream>
#include <algorithm>

// g++ --std=c++17 -O2 -pg -g code.cpp

void fill_arrays(double* x, double* a, double* b, int count){
    std::fill(x, x+count, 0.0);
    std::fill(a, a+count, 1.0);
    std::fill(b, b+count, 2.0);
}

void print(double* x, int count){
    for(int i = 0; i<count; i++){
        std::cout << x[i] << ", ";
    }
    std::cout << std::endl;
}

void single_thread(int count){
    double* x = new double[count];
    double* a = new double[count];
    double* b = new double[count];
    fill_arrays(x,a,b,count);

    for(int i = 0; i < count-1; i++){
        x[i+1] = a[i]*x[i] + b[i];
    }

    // print(x, count);
    int res = x[count - 1];
    delete[] x;
    delete[] a;
    delete[] b;
}

void multi_thread_1(int count){
    double* x = new double[count];
    double* a = new double[count];
    double* b = new double[count];
    fill_arrays(x,a,b,count);


    for(int i = 0; i < count-2; i += 2){
        int xi = a[i]*x[i] + b[i];
        x[i+2] = a[i+1]*(xi) + b[i+1];
    }

    x[1] = a[1]*x[0] + b[1];
    for(int i = 1; i < count-2; i += 2){
        int xi = a[i]*x[i] + b[i];
        x[i+2] = a[i+1]*(xi) + b[i+1];
    }

    delete[] x;
    delete[] a;
    delete[] b;
}

void multi_thread_2(int count) {
    double* x = new double[count];
    double* a = new double[count];
    double* b = new double[count];
    double* x_result = new double[count];
    fill_arrays(x, a, b, count);

    x[1] = a[0] * x[0] + b[0];
    x_result[1] = a[1] * x[1] + b[1];
    x_result[0] = x[0];

    for (int step = 2; step < 4; ++step) {
        for (int i = step; i < count; i += 2) {
            double a_i = a[i-1];
            double b_i = b[i-1];
            double res = a_i * x_result[i-2] + b_i;
            x[i] = res;
            x_result[i] = a_i * res + b_i;
        }
    }

    delete[] x;
    delete[] a;
    delete[] b;
    delete[] x_result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <COUNT>" << std::endl;
        return 1;
    }

    int COUNT = std::stoi(argv[1]);
    if (COUNT <= 0) {
        std::cerr << "Please provide a positive integer value for COUNT." << std::endl;
        return 1;
    }

    multi_thread_1(COUNT);
    multi_thread_2(COUNT);
    single_thread(COUNT);

    return 0;
}