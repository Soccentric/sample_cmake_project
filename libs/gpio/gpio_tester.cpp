/**
 * @file gpio_tester.cpp
 * @brief Implementation of GPIO peripheral tester.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 *
 * This file implements the GPIOTester class methods for comprehensive
 * GPIO testing on Raspberry Pi CM5.
 */

#include "gpio_tester.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cstring>
#include <errno.h>
#include <unistd.h>

namespace cm5_peripheral_test {

namespace fs = std::filesystem;

GPIOTester::GPIOTester() : gpio_available_(false) {
    // Check if GPIO sysfs is available
    gpio_available_ = fs::exists("/sys/class/gpio");

    // Initialize test pins for CM5
    // GPIO pins available on CM5 (based on 40-pin HAT compatible header)
    test_pins_ = {
        {2, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 2
        {3, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 3
        {4, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 4
        {5, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 5
        {6, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 6
        {7, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 7
        {8, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 8
        {9, GPIOMode::OUTPUT, false, false, 0, 0},    // GPIO 9
        {10, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 10
        {11, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 11
        {12, GPIOMode::PWM, false, false, 1000, 50},  // GPIO 12 (PWM0)
        {13, GPIOMode::PWM, false, false, 1000, 50},  // GPIO 13 (PWM1)
        {14, GPIOMode::UART, false, false, 0, 0},     // GPIO 14 (UART TX)
        {15, GPIOMode::UART, false, false, 0, 0},     // GPIO 15 (UART RX)
        {16, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 16
        {17, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 17
        {18, GPIOMode::PWM, false, false, 1000, 50},  // GPIO 18 (PWM0)
        {19, GPIOMode::SPI, false, false, 0, 0},      // GPIO 19 (SPI MOSI)
        {20, GPIOMode::SPI, false, false, 0, 0},      // GPIO 20 (SPI MISO)
        {21, GPIOMode::SPI, false, false, 0, 0},      // GPIO 21 (SPI SCLK)
        {22, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 22
        {23, GPIOMode::SPI, false, false, 0, 0},      // GPIO 23 (SPI CE0)
        {24, GPIOMode::SPI, false, false, 0, 0},      // GPIO 24 (SPI CE1)
        {25, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 25
        {26, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 26
        {27, GPIOMode::OUTPUT, false, false, 0, 0},   // GPIO 27
    };
}

GPIOTester::~GPIOTester() {
    // Cleanup: unexport any exported GPIOs
    for (const auto& pin : test_pins_) {
        unexport_gpio(pin.number);
    }
}

TestReport GPIOTester::short_test() {
    auto start_time = std::chrono::steady_clock::now();

    if (!gpio_available_) {
        return create_report(TestResult::NOT_SUPPORTED, "GPIO sysfs interface not available", std::chrono::milliseconds(0));
    }

    std::stringstream details;
    bool all_passed = true;

    // Test digital I/O
    TestResult digital_result = test_digital_io();
    details << "Digital I/O: " << (digital_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (digital_result != TestResult::SUCCESS) all_passed = false;

    // Test PWM
    TestResult pwm_result = test_pwm();
    details << "PWM: " << (pwm_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (pwm_result != TestResult::SUCCESS) all_passed = false;

    // Test I2C
    TestResult i2c_result = test_i2c();
    details << "I2C: " << (i2c_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (i2c_result != TestResult::SUCCESS) all_passed = false;

    // Test SPI
    TestResult spi_result = test_spi();
    details << "SPI: " << (spi_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (spi_result != TestResult::SUCCESS) all_passed = false;

    // Test UART
    TestResult uart_result = test_uart();
    details << "UART: " << (uart_result == TestResult::SUCCESS ? "PASS" : "FAIL") << "\n";
    if (uart_result != TestResult::SUCCESS) all_passed = false;

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    TestResult overall_result = all_passed ? TestResult::SUCCESS : TestResult::FAILURE;
    return create_report(overall_result, details.str(), duration);
}

TestReport GPIOTester::monitor_test(std::chrono::seconds duration) {
    auto start_time = std::chrono::steady_clock::now();

    if (!gpio_available_) {
        return create_report(TestResult::NOT_SUPPORTED, "GPIO sysfs interface not available", std::chrono::milliseconds(0));
    }

    TestResult result = monitor_gpio_stability(duration);

    auto end_time = std::chrono::steady_clock::now();
    auto test_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::string details = "GPIO monitoring completed for " + std::to_string(duration.count()) + " seconds";
    return create_report(result, details, test_duration);
}

bool GPIOTester::is_available() const {
    return gpio_available_;
}

TestResult GPIOTester::test_digital_io() {
    // Test a few GPIO pins for digital I/O
    std::vector<int> test_gpios = {2, 3, 4}; // Safe pins to test

    for (int gpio : test_gpios) {
        // Export GPIO
        if (!export_gpio(gpio)) {
            return TestResult::FAILURE;
        }

        // Set as output
        if (!set_gpio_direction(gpio, true)) {
            unexport_gpio(gpio);
            return TestResult::FAILURE;
        }

        // Test writing high
        if (!write_gpio(gpio, 1)) {
            unexport_gpio(gpio);
            return TestResult::FAILURE;
        }

        // Small delay
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Test writing low
        if (!write_gpio(gpio, 0)) {
            unexport_gpio(gpio);
            return TestResult::FAILURE;
        }

        // Set as input
        if (!set_gpio_direction(gpio, false)) {
            unexport_gpio(gpio);
            return TestResult::FAILURE;
        }

        // Test reading
        int value = read_gpio(gpio);
        if (value == -1) {
            unexport_gpio(gpio);
            return TestResult::FAILURE;
        }

        // Unexport GPIO
        unexport_gpio(gpio);
    }

    return TestResult::SUCCESS;
}

TestResult GPIOTester::test_pwm() {
    // Test PWM on GPIO 18 (PWM0)
    int pwm_gpio = 18;

    // Export GPIO
    if (!export_gpio(pwm_gpio)) {
        return TestResult::FAILURE;
    }

    // For PWM testing, we would need to check if PWM sysfs is available
    // This is a simplified test - in practice, PWM setup requires device tree overlays
    std::string pwm_path = "/sys/class/pwm/pwmchip0";
    if (!fs::exists(pwm_path)) {
        unexport_gpio(pwm_gpio);
        return TestResult::NOT_SUPPORTED;
    }

    // Unexport GPIO
    unexport_gpio(pwm_gpio);

    return TestResult::SUCCESS;
}

TestResult GPIOTester::test_i2c() {
    // Check if I2C devices are available
    std::vector<std::string> i2c_devices = {"/dev/i2c-0", "/dev/i2c-1"};

    bool i2c_found = false;
    for (const auto& device : i2c_devices) {
        if (fs::exists(device)) {
            i2c_found = true;
            break;
        }
    }

    if (!i2c_found) {
        return TestResult::NOT_SUPPORTED;
    }

    // In a full implementation, we would test I2C communication
    // For now, just check device presence
    return TestResult::SUCCESS;
}

TestResult GPIOTester::test_spi() {
    // Check if SPI devices are available
    std::vector<std::string> spi_devices = {"/dev/spidev0.0", "/dev/spidev0.1"};

    bool spi_found = false;
    for (const auto& device : spi_devices) {
        if (fs::exists(device)) {
            spi_found = true;
            break;
        }
    }

    if (!spi_found) {
        return TestResult::NOT_SUPPORTED;
    }

    // In a full implementation, we would test SPI communication
    return TestResult::SUCCESS;
}

TestResult GPIOTester::test_uart() {
    // Check if UART devices are available
    std::vector<std::string> uart_devices = {"/dev/ttyAMA0", "/dev/ttyS0"};

    bool uart_found = false;
    for (const auto& device : uart_devices) {
        if (fs::exists(device)) {
            uart_found = true;
            break;
        }
    }

    if (!uart_found) {
        return TestResult::NOT_SUPPORTED;
    }

    // In a full implementation, we would test UART communication
    return TestResult::SUCCESS;
}

TestResult GPIOTester::monitor_gpio_stability(std::chrono::seconds duration) {
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + duration;

    int test_gpio = 2; // Use GPIO 2 for monitoring

    // Export and set as input
    if (!export_gpio(test_gpio) || !set_gpio_direction(test_gpio, false)) {
        return TestResult::FAILURE;
    }

    int stable_count = 0;
    int total_reads = 0;

    while (std::chrono::steady_clock::now() < end_time) {
        int value = read_gpio(test_gpio);
        if (value != -1) {
            stable_count++;
        }
        total_reads++;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Unexport GPIO
    unexport_gpio(test_gpio);

    // Consider it stable if 95% of reads succeeded
    double stability_ratio = static_cast<double>(stable_count) / total_reads;
    return (stability_ratio >= 0.95) ? TestResult::SUCCESS : TestResult::FAILURE;
}

bool GPIOTester::export_gpio(int pin) {
    std::ofstream export_file("/sys/class/gpio/export");
    if (!export_file.is_open()) {
        return false;
    }

    export_file << pin;
    export_file.close();

    // Wait a bit for the GPIO to be exported
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return fs::exists("/sys/class/gpio/gpio" + std::to_string(pin));
}

bool GPIOTester::unexport_gpio(int pin) {
    std::ofstream unexport_file("/sys/class/gpio/unexport");
    if (!unexport_file.is_open()) {
        return false;
    }

    unexport_file << pin;
    return true;
}

bool GPIOTester::set_gpio_direction(int pin, bool output) {
    std::string direction_path = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
    std::ofstream direction_file(direction_path);
    if (!direction_file.is_open()) {
        return false;
    }

    direction_file << (output ? "out" : "in");
    return true;
}

int GPIOTester::read_gpio(int pin) {
    std::string value_path = "/sys/class/gpio/gpio" + std::to_string(pin) + "/value";
    std::ifstream value_file(value_path);
    if (!value_file.is_open()) {
        return -1;
    }

    int value;
    value_file >> value;
    return value;
}

bool GPIOTester::write_gpio(int pin, int value) {
    std::string value_path = "/sys/class/gpio/gpio" + std::to_string(pin) + "/value";
    std::ofstream value_file(value_path);
    if (!value_file.is_open()) {
        return false;
    }

    value_file << value;
    return true;
}

} // namespace cm5_peripheral_test