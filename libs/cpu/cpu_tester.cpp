/**
 * @file cpu_tester.cpp
 * @brief Implementation of CPU peripheral tester.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 */

#include "cpu_tester.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace fs = std::filesystem;

namespace cm5_peripheral_test {

CPUTester::CPUTester() : cpu_available_(false) {
    // Check if CPU information is available
    cpu_available_ = fs::exists("/proc/cpuinfo");
    if (cpu_available_) {
        cpu_info_ = get_cpu_info();
    }
}

TestReport CPUTester::short_test() {
    auto start_time = std::chrono::steady_clock::now();

    if (!cpu_available_) {
        return create_report(TestResult::NOT_SUPPORTED, "CPU information not available", std::chrono::milliseconds(0));
    }

    std::stringstream details;
    bool all_passed = true;

    // Test CPU info retrieval
    details << "CPU Model: " << cpu_info_.model_name << "\n";
    details << "Cores: " << cpu_info_.cores << "\n";
    details << "Architecture: " << cpu_info_.architecture << "\n";
    details << "Frequency: " << cpu_info_.frequency_mhz << " MHz\n";

    // Test basic computation
    TestResult benchmark_result = benchmark_cpu();
    details << "Benchmark: " << (benchmark_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (benchmark_result != TestResult::SUCCESS) all_passed = false;

    // Test temperature
    TestResult temp_result = test_temperature();
    details << "Temperature: " << (temp_result == TestResult::SUCCESS ? "PASS" : "FAIL");
    if (temp_result == TestResult::SUCCESS) {
        details << " (" << cpu_info_.temperature_c << "°C)\n";
    } else {
        details << "\n";
    }
    if (temp_result != TestResult::SUCCESS && temp_result != TestResult::NOT_SUPPORTED) all_passed = false;

    // Test multi-core
    TestResult multi_core_result = test_multi_core();
    details << "Multi-core: " << (multi_core_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (multi_core_result != TestResult::SUCCESS) all_passed = false;

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    TestResult overall_result = all_passed ? TestResult::SUCCESS : TestResult::FAILURE;
    return create_report(overall_result, details.str(), duration);
}

TestReport CPUTester::monitor_test(std::chrono::seconds duration) {
    auto start_time = std::chrono::steady_clock::now();

    if (!cpu_available_) {
        return create_report(TestResult::NOT_SUPPORTED, "CPU information not available", std::chrono::milliseconds(0));
    }

    TestResult result = monitor_temperature(duration);

    auto end_time = std::chrono::steady_clock::now();
    auto test_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::string details = "CPU monitoring completed for " + std::to_string(duration.count()) + " seconds";
    return create_report(result, details, test_duration);
}

bool CPUTester::is_available() const {
    return cpu_available_;
}

CPUInfo CPUTester::get_cpu_info() {
    CPUInfo info;
    std::ifstream cpuinfo("/proc/cpuinfo");

    if (!cpuinfo.is_open()) {
        return info;
    }

    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                info.model_name = line.substr(colon_pos + 2);
            }
        } else if (line.find("cpu cores") != std::string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                info.cores = std::stoi(line.substr(colon_pos + 2));
            }
        } else if (line.find("CPU architecture") != std::string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                info.architecture = line.substr(colon_pos + 2);
            }
        }
    }

    // Get CPU frequency
    std::ifstream freq_file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (freq_file.is_open()) {
        std::string freq_str;
        std::getline(freq_file, freq_str);
        try {
            info.frequency_mhz = std::stod(freq_str) / 1000.0;
        } catch (...) {
            info.frequency_mhz = 0.0;
        }
    }

    // Get temperature
    info.temperature_c = get_cpu_temperature();

    return info;
}

TestResult CPUTester::benchmark_cpu() {
    // Simple CPU benchmark: calculate prime numbers
    const int MAX_PRIME = 10000;
    std::vector<int> primes;

    for (int num = 2; num <= MAX_PRIME; ++num) {
        bool is_prime = true;
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes.push_back(num);
        }
    }

    // Verify we found some primes
    if (primes.empty() || primes.back() != 9973) { // 10000th prime is 104729, but we use smaller
        return TestResult::FAILURE;
    }

    return TestResult::SUCCESS;
}

TestResult CPUTester::test_temperature() {
    double temp = get_cpu_temperature();
    if (temp < 0) {
        return TestResult::NOT_SUPPORTED;
    }

    // Check if temperature is reasonable (0-100°C)
    if (temp < 0 || temp > 100) {
        return TestResult::FAILURE;
    }

    return TestResult::SUCCESS;
}

TestResult CPUTester::monitor_temperature(std::chrono::seconds duration) {
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + duration;

    std::vector<double> temperatures;
    double min_temp = 999.0;
    double max_temp = -999.0;

    while (std::chrono::steady_clock::now() < end_time) {
        double temp = get_cpu_temperature();
        if (temp >= 0) {
            temperatures.push_back(temp);
            min_temp = std::min(min_temp, temp);
            max_temp = std::max(max_temp, temp);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (temperatures.empty()) {
        return TestResult::NOT_SUPPORTED;
    }

    // Check temperature stability (variation should be reasonable)
    double avg_temp = std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / temperatures.size();
    double temp_variation = max_temp - min_temp;

    // Allow up to 20°C variation during monitoring
    return (temp_variation <= 20.0) ? TestResult::SUCCESS : TestResult::FAILURE;
}

TestResult CPUTester::test_multi_core() {
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        return TestResult::NOT_SUPPORTED;
    }

    // Basic multi-threading test
    std::vector<std::thread> threads;
    std::vector<int> results(num_threads, 0);

    for (unsigned int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i, &results]() {
            // Simple computation per thread
            int sum = 0;
            for (int j = 0; j < 1000; ++j) {
                sum += j * i;
            }
            results[i] = sum;
        });
    }

    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Verify all threads completed
    for (int result : results) {
        if (result == 0) {
            return TestResult::FAILURE;
        }
    }

    return TestResult::SUCCESS;
}

double CPUTester::get_cpu_temperature() {
    // Try different temperature sensor locations
    std::vector<std::string> temp_files = {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/hwmon/hwmon0/temp1_input",
        "/proc/acpi/thermal_zone/THM0/temperature"
    };

    for (const auto& temp_file : temp_files) {
        std::ifstream temp_stream(temp_file);
        if (temp_stream.is_open()) {
            std::string temp_str;
            std::getline(temp_stream, temp_str);
            try {
                double temp = std::stod(temp_str);
                // Convert millidegrees to degrees if necessary
                if (temp > 1000) {
                    temp /= 1000.0;
                }
                return temp;
            } catch (...) {
                continue;
            }
        }
    }

    return -1.0; // Not available
}

} // namespace cm5_peripheral_test