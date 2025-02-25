#pragma once

#include <mpi.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @file PerformanceMonitor.hpp
 * @brief Singleton class for timing various parts of the Agoge solver.
 */

namespace agoge {

/**
 * @struct TimerData
 * @brief Stores total accumulated time and call count for a particular timer.
 */
struct TimerData {
    std::chrono::steady_clock::duration total{};
    long callCount = 0;
};

/**
 * @class PerformanceMonitor
 * @brief A singleton class that manages named timers for performance tracking.
 *
 * Usage:
 *   PerformanceMonitor::instance().startTimer("computeL");
 *   // ... code ...
 *   PerformanceMonitor::instance().stopTimer("computeL");
 *
 *   At program end:
 *   PerformanceMonitor::instance().setSteps(steps);
 *   PerformanceMonitor::instance().setZones(zones);
 *   PerformanceMonitor::instance().compileReport();
 */
class PerformanceMonitor {
   public:
    /**
     * @brief Access the global singleton instance of PerformanceMonitor.
     */
    static PerformanceMonitor &instance() {
        static PerformanceMonitor s_instance;
        return s_instance;
    }

    /**
     * @brief Start a timer for the given name. If already running, it restarts.
     *
     * @param name Unique string identifying this timer (e.g., "computeL").
     */
    void startTimer(const std::string &name) {
        auto now = std::chrono::steady_clock::now();
        startTimes_[name] = now;
    }

    /**
     * @brief Stop a timer for the given name, accumulate the elapsed time.
     *
     * @param name Must match a previously started timer.
     */
    void stopTimer(const std::string &name) {
        auto now = std::chrono::steady_clock::now();
        auto it = startTimes_.find(name);
        if (it != startTimes_.end()) {
            auto elapsed = now - it->second;
            timers_[name].total += elapsed;
            timers_[name].callCount += 1;
            // Optionally erase the start time
            // startTimes_.erase(it);
        } else {
            std::cerr << "[PerformanceMonitor] Warning: Timer '" << name
                      << "' was not started.\n";
        }
    }

    /**
     * @brief Set the total number of simulation steps.
     *
     * This value is used to compute zone updates per second.
     *
     * @param steps Total number of simulation steps taken.
     */
    void setSteps(long steps) { steps_ = steps; }

    /**
     * @brief Set the total number of zones in the simulation domain.
     *
     * This value is used to compute zone updates per second.
     *
     * @param zones Total number of zones in the domain.
     */
    void setZones(long zones) { zones_ = zones; }

    /**
     * @brief Set the processor rank.
     *
     * @param rank Processor rank.
     */
    void setRank(int rank) { rank_ = rank; }

    /**
     * @brief Set the total number of ranks.
     *
     * @param size Total number of ranks.
     */
    void setCommSize(int size) { size_ = size; }

    /**
     * @brief Compile and print a summary of all timers.
     *
     * All MPI collectives within this method are called by every rank.
     * However, the detailed report is printed only by rank 0.
     */
    void compileReport() const {
        // Gather local timer names and data. All ranks participate.
        std::vector<std::string> timerNames;
        for (const auto &pair : timers_) {
            timerNames.push_back(pair.first);
        }
        int count = timerNames.size();
        std::vector<double> localTotals(count);
        std::vector<double> localCalls(count);
        int idx = 0;
        for (const auto &pair : timers_) {
            localTotals[idx] = std::chrono::duration<double>(pair.second.total).count();
            localCalls[idx] = static_cast<double>(pair.second.callCount);
            idx++;
        }

        std::vector<double> minTotals(count);
        std::vector<double> maxTotals(count);
        std::vector<double> sumTotals(count);

        MPI_Allreduce(localTotals.data(), minTotals.data(), count, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        MPI_Allreduce(localTotals.data(), maxTotals.data(), count, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        MPI_Allreduce(localTotals.data(), sumTotals.data(), count, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        if (rank_ == 0) {
            printAggregatedReport(timerNames, minTotals, maxTotals, sumTotals, localCalls);
        }
    }

   private:
    PerformanceMonitor() = default;  // Private constructor for singleton
    PerformanceMonitor(const PerformanceMonitor &) = delete;
    PerformanceMonitor &operator=(const PerformanceMonitor &) = delete;

    // Aggregated report with min, max, and average across all ranks
    void printAggregatedReport(const std::vector<std::string>& timerNames,
                               const std::vector<double>& minTotals,
                               const std::vector<double>& maxTotals,
                               const std::vector<double>& sumTotals,
                               const std::vector<double>& localCalls) const {
        const int headerWidth = 70;
        const int nameWidth  = 24;
        const int colWidth   = 12;
        std::cout << "\n====== AGGREGATED TIMINGS ACROSS ALL RANKS ======\n\n";
        std::cout << std::left << std::setw(nameWidth) << "Timer"
                  << std::right << std::setw(colWidth) << "Min(s)"
                  << std::right << std::setw(colWidth) << "Max(s)"
                  << std::right << std::setw(colWidth) << "Avg(s)"
                  << std::right << std::setw(colWidth) << "Calls"
                  << "\n";
        std::cout << std::string(headerWidth, '-') << "\n";

        int size = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        double zoneUpdates = 0.0;
        for (size_t i = 0; i < timerNames.size(); i++) {
            double avgTotal = sumTotals[i] / static_cast<double>(size);
            if (timerNames[i] == "timeLoop") {
                zoneUpdates = (maxTotals[i] > 0.0)
                                  ? (static_cast<double>(steps_) * zones_ / maxTotals[i] / 1e6)
                                  : 0.0;
            }
            std::cout << std::left << std::setw(nameWidth) << timerNames[i]
                      << std::right << std::setw(colWidth) << std::fixed << std::setprecision(6) << minTotals[i]
                      << std::right << std::setw(colWidth) << maxTotals[i]
                      << std::right << std::setw(colWidth) << avgTotal
                      << std::right << std::setw(colWidth) << (long)(localCalls[i])
                      << "\n";
        }
        std::cout << std::string(headerWidth, '-') << "\n\n";
        std::cout << "Zone Updates per Second (M): " << std::fixed << std::setprecision(2) << zoneUpdates << "\n\n";
    }

    // Map from timer name -> accumulated data
    std::unordered_map<std::string, TimerData> timers_;

    // Map from timer name -> start time
    std::unordered_map<std::string, std::chrono::steady_clock::time_point>
        startTimes_;

    // Simulation parameters for zone updates
    long steps_ = 0;
    long zones_ = 0;
    int rank_ = 0;  // New member to store the processor rank
    int size_ = 1;  // store total number of ranks
};

}  // namespace agoge
