/**
 * @file main.cpp
 * @brief GPIO peripheral test application.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 *
 * This application provides command-line interface for testing GPIO
 * peripherals on Raspberry Pi CM5.
 */

#include "gpio_tester.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace cm5_peripheral_test;

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << "  --short          Run short GPIO test\n"
              << "  --monitor <sec>  Run monitoring test for specified seconds\n"
              << "  --help           Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    GPIOTester tester;

    if (!tester.is_available()) {
        std::cerr << "GPIO peripheral is not available on this system.\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "--short") {
        std::cout << "Running GPIO short test...\n";
        TestReport report = tester.short_test();

        std::cout << "Test Result: " << (report.result == TestResult::SUCCESS ? "SUCCESS" : "FAILURE") << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n";
        std::cout << "Details:\n" << report.details << "\n";

        return report.result == TestResult::SUCCESS ? 0 : 1;

    } else if (command == "--monitor" && argc >= 3) {
        int seconds = std::stoi(argv[2]);
        std::cout << "Running GPIO monitoring test for " << seconds << " seconds...\n";

        TestReport report = tester.monitor_test(std::chrono::seconds(seconds));

        std::cout << "Test Result: " << (report.result == TestResult::SUCCESS ? "SUCCESS" : "FAILURE") << "\n";
        std::cout << "Duration: " << report.duration.count() << " ms\n";
        std::cout << "Details:\n" << report.details << "\n";

        return report.result == TestResult::SUCCESS ? 0 : 1;

    } else if (command == "--help") {
        print_usage(argv[0]);
        return 0;
    } else {
        std::cerr << "Invalid command. Use --help for usage information.\n";
        return 1;
    }

    return 0;
}