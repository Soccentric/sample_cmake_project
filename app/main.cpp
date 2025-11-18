/**
 * @file main.cpp
 * @brief Main application entry point for CM5 peripheral verification tool.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 *
 * This file contains the main() function that serves as the entry point
 * for the Raspberry Pi Compute Module 5 hardware peripheral verification tool.
 * It orchestrates testing of all hardware peripherals.
 *
 * @version 1.0
 * @date 2025-11-17
 *
 * @details
 * The application provides a comprehensive testing framework for all CM5
 * hardware peripherals including CPU, GPU, memory, storage, display, camera,
 * USB, networking, GPIO, and power management systems.
 *
 * Command-line options allow selection of specific peripherals and test modes.
 */

#include "gpio_tester.h"
#include "cpu_tester.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>

using namespace cm5_peripheral_test;

/**
 * @brief Prints usage information for the application.
 * @param program_name The name of the executable.
 */
void print_usage(const char* program_name) {
    std::cout << "Raspberry Pi Compute Module 5 Hardware Peripheral Verification Tool\n"
              << "Usage: " << program_name << " [options]\n\n"
              << "Options:\n"
              << "  --all-short          Run short tests for all peripherals\n"
              << "  --all-monitor <sec>  Run monitoring tests for all peripherals\n"
              << "  --cpu-short          Run short CPU test\n"
              << "  --cpu-monitor <sec>  Run CPU monitoring test\n"
              << "  --gpio-short         Run short GPIO test\n"
              << "  --gpio-monitor <sec> Run GPIO monitoring test\n"
              << "  --list               List all available peripherals\n"
              << "  --help               Show this help message\n\n"
              << "Examples:\n"
              << "  " << program_name << " --all-short\n"
              << "  " << program_name << " --cpu-monitor 60\n"
              << "  " << program_name << " --list\n";
}

/**
 * @brief Lists all available peripherals and their status.
 */
void list_peripherals() {
    std::cout << "Available Peripherals:\n";
    std::cout << "=====================\n";

    // CPU
    CPUTester cpu_tester;
    std::cout << "CPU: " << (cpu_tester.is_available() ? "Available" : "Not Available") << "\n";

    // GPIO
    GPIOTester gpio_tester;
    std::cout << "GPIO: " << (gpio_tester.is_available() ? "Available" : "Not Available") << "\n";

    // TODO: Add other peripherals as they are implemented
    std::cout << "\nNote: Only CPU and GPIO are currently implemented.\n";
    std::cout << "More peripherals will be added in future versions.\n";
}

/**
 * @brief Runs short tests for all available peripherals.
 * @return 0 on success, non-zero on failure.
 */
int run_all_short_tests() {
    std::cout << "Running short tests for all peripherals...\n\n";

    int failed_tests = 0;

    // CPU test
    CPUTester cpu_tester;
    if (cpu_tester.is_available()) {
        std::cout << "Testing CPU...\n";
        TestReport report = cpu_tester.short_test();
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details: " << report.details << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n\n";

        if (report.result != TestResult::SUCCESS) {
            failed_tests++;
        }
    } else {
        std::cout << "CPU: Not available, skipping...\n\n";
    }

    // GPIO test
    GPIOTester gpio_tester;
    if (gpio_tester.is_available()) {
        std::cout << "Testing GPIO...\n";
        TestReport report = gpio_tester.short_test();
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details: " << report.details << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n\n";

        if (report.result != TestResult::SUCCESS) {
            failed_tests++;
        }
    } else {
        std::cout << "GPIO: Not available, skipping...\n\n";
    }

    // TODO: Add other peripherals

    if (failed_tests == 0) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << failed_tests << " test(s) failed.\n";
        return 1;
    }
}

/**
 * @brief Runs monitoring tests for all available peripherals.
 * @param duration_seconds Duration for monitoring in seconds.
 * @return 0 on success, non-zero on failure.
 */
int run_all_monitor_tests(int duration_seconds) {
    std::cout << "Running monitoring tests for all peripherals (" << duration_seconds << " seconds)...\n\n";

    int failed_tests = 0;

    // CPU test
    CPUTester cpu_tester;
    if (cpu_tester.is_available()) {
        std::cout << "Monitoring CPU...\n";
        TestReport report = cpu_tester.monitor_test(std::chrono::seconds(duration_seconds));
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details: " << report.details << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n\n";

        if (report.result != TestResult::SUCCESS) {
            failed_tests++;
        }
    } else {
        std::cout << "CPU: Not available, skipping...\n\n";
    }

    // GPIO test
    GPIOTester gpio_tester;
    if (gpio_tester.is_available()) {
        std::cout << "Monitoring GPIO...\n";
        TestReport report = gpio_tester.monitor_test(std::chrono::seconds(duration_seconds));
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details: " << report.details << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n\n";

        if (report.result != TestResult::SUCCESS) {
            failed_tests++;
        }
    } else {
        std::cout << "GPIO: Not available, skipping...\n\n";
    }

    // TODO: Add other peripherals

    if (failed_tests == 0) {
        std::cout << "All monitoring tests passed!\n";
        return 0;
    } else {
        std::cout << failed_tests << " monitoring test(s) failed.\n";
        return 1;
    }
}

/**
 * @brief Main entry point of the application.
 *
 * Parses command-line arguments and executes the requested tests.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on successful execution, non-zero on error.
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "--all-short") {
        return run_all_short_tests();

    } else if (command == "--all-monitor" && argc >= 3) {
        try {
            int seconds = std::stoi(argv[2]);
            if (seconds <= 0) {
                std::cerr << "Error: Duration must be positive.\n";
                return 1;
            }
            return run_all_monitor_tests(seconds);
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid duration value.\n";
            return 1;
        }

    } else if (command == "--cpu-short") {
        CPUTester tester;
        if (!tester.is_available()) {
            std::cerr << "CPU peripheral is not available on this system.\n";
            return 1;
        }

        std::cout << "Running CPU short test...\n";
        TestReport report = tester.short_test();
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details:\n" << report.details << "\n";
        return report.result == TestResult::SUCCESS ? 0 : 1;

    } else if (command == "--cpu-monitor" && argc >= 3) {
        try {
            int seconds = std::stoi(argv[2]);
            if (seconds <= 0) {
                std::cerr << "Error: Duration must be positive.\n";
                return 1;
            }

            CPUTester tester;
            if (!tester.is_available()) {
                std::cerr << "CPU peripheral is not available on this system.\n";
                return 1;
            }

            std::cout << "Running CPU monitoring test for " << seconds << " seconds...\n";
            TestReport report = tester.monitor_test(std::chrono::seconds(seconds));
            std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
            std::cout << "Details:\n" << report.details << "\n";
            return report.result == TestResult::SUCCESS ? 0 : 1;
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid duration value.\n";
            return 1;
        }
        GPIOTester tester;
        if (!tester.is_available()) {
            std::cerr << "GPIO peripheral is not available on this system.\n";
            return 1;
        }

        std::cout << "Running GPIO short test...\n";
        TestReport report = tester.short_test();
        std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
        std::cout << "Details:\n" << report.details << "\n";
        return report.result == TestResult::SUCCESS ? 0 : 1;

    } else if (command == "--gpio-monitor" && argc >= 3) {
        try {
            int seconds = std::stoi(argv[2]);
            if (seconds <= 0) {
                std::cerr << "Error: Duration must be positive.\n";
                return 1;
            }

            GPIOTester tester;
            if (!tester.is_available()) {
                std::cerr << "GPIO peripheral is not available on this system.\n";
                return 1;
            }

            std::cout << "Running GPIO monitoring test for " << seconds << " seconds...\n";
            TestReport report = tester.monitor_test(std::chrono::seconds(seconds));
            std::cout << "Result: " << (report.result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
            std::cout << "Details:\n" << report.details << "\n";
            return report.result == TestResult::SUCCESS ? 0 : 1;
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid duration value.\n";
            return 1;
        }

    } else if (command == "--list") {
        list_peripherals();
        return 0;

    } else if (command == "--help") {
        print_usage(argv[0]);
        return 0;

    } else {
        std::cerr << "Unknown command: " << command << "\n";
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}