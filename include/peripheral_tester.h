/**
 * @file peripheral_tester.h
 * @brief Base class for all peripheral testers in the Raspberry Pi CM5 verification tool.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 *
 * This header defines the base interface for testing and monitoring all hardware
 * peripherals on the Raspberry Pi Compute Module 5. Each peripheral implements
 * this interface to provide standardized testing capabilities.
 *
 * @version 1.0
 * @date 2025-11-17
 *
 * @details
 * The PeripheralTester class serves as an abstract base class that defines the
 * contract for all peripheral testing implementations. It provides two primary
 * testing modes:
 * - Short testing: Quick verification of peripheral functionality
 * - Monitoring: Extended testing over a specified duration
 *
 * All derived classes must implement the pure virtual methods to ensure
 * consistent behavior across all peripherals.
 *
 * @note This interface is designed to be extensible for future peripheral additions.
 * @note All implementations should be thread-safe where possible.
 */

#ifndef PERIPHERAL_TESTER_H
#define PERIPHERAL_TESTER_H

#include <string>
#include <chrono>
#include <memory>

/**
 * @namespace cm5_peripheral_test
 * @brief Main namespace for the CM5 peripheral testing framework.
 *
 * This namespace encapsulates all classes, functions, and utilities related to
 * testing and verifying Raspberry Pi Compute Module 5 hardware peripherals.
 */
namespace cm5_peripheral_test {

/**
 * @enum TestResult
 * @brief Enumeration of possible test outcomes.
 *
 * This enum defines the standard result codes that all peripheral tests
 * should return to indicate their success or failure status.
 */
enum class TestResult {
    SUCCESS,        /**< Test completed successfully */
    FAILURE,        /**< Test failed due to hardware or software error */
    NOT_SUPPORTED,  /**< Peripheral is not supported on this hardware */
    TIMEOUT,        /**< Test exceeded the allocated time limit */
    SKIPPED         /**< Test was intentionally skipped */
};

/**
 * @struct TestReport
 * @brief Structure containing detailed test results and metadata.
 *
 * This structure encapsulates the complete results of a peripheral test,
 * including success status, timing information, and diagnostic data.
 */
struct TestReport {
    TestResult result;                           /**< Overall test outcome */
    std::string peripheral_name;                 /**< Name of the peripheral tested */
    std::chrono::milliseconds duration;          /**< Time taken to complete the test */
    std::string details;                         /**< Detailed test output or error messages */
    std::chrono::system_clock::time_point timestamp; /**< When the test was executed */

    /**
     * @brief Default constructor initializing all fields.
     */
    TestReport() : result(TestResult::SKIPPED), duration(0), timestamp(std::chrono::system_clock::now()) {}
};

/**
 * @class PeripheralTester
 * @brief Abstract base class for all peripheral testing implementations.
 *
 * This class defines the interface that all peripheral testers must implement.
 * It provides a standardized way to perform short verification tests and
 * extended monitoring of hardware peripherals.
 *
 * @details
 * Derived classes should implement the pure virtual methods to provide
 * peripheral-specific testing logic. The base class handles common functionality
 * such as timing and result reporting.
 *
 * @note Implementations should be designed to be reusable across different
 *       testing scenarios and applications.
 *
 * @thread_safety Implementations should document their thread-safety guarantees.
 */
class PeripheralTester {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     *
     * Ensures that derived class destructors are called correctly when
     * objects are deleted through base class pointers.
     */
    virtual ~PeripheralTester() = default;

    /**
     * @brief Performs a short verification test of the peripheral.
     *
     * This method executes a quick test to verify that the peripheral is
     * functioning correctly. The test should complete in a reasonable time
     * (typically seconds) and provide basic assurance of hardware integrity.
     *
     * @return TestReport containing the results of the short test.
     *
     * @throws std::runtime_error if the test encounters a critical error
     *         that prevents completion.
     *
     * @note This method should be idempotent and not leave the peripheral
     *       in an altered state.
     *
     * @see monitor_test()
     */
    virtual TestReport short_test() = 0;

    /**
     * @brief Performs extended monitoring of the peripheral.
     *
     * This method executes a longer-duration test that monitors the peripheral
     * over time to detect stability issues, performance degradation, or
     * intermittent failures. The test runs for the specified duration.
     *
     * @param duration The time period over which to monitor the peripheral.
     *
     * @return TestReport containing the results of the monitoring test.
     *
     * @throws std::runtime_error if the test encounters a critical error
     *         that prevents completion.
     * @throws std::invalid_argument if duration is invalid (e.g., zero or negative).
     *
     * @note This method may block for the entire duration of the test.
     * @note Implementations should provide progress updates where possible.
     *
     * @see short_test()
     */
    virtual TestReport monitor_test(std::chrono::seconds duration) = 0;

    /**
     * @brief Returns the name of the peripheral being tested.
     *
     * Provides a human-readable name for the peripheral, useful for
     * logging, reporting, and user interface display.
     *
     * @return std::string containing the peripheral name.
     *
     * @note The name should be descriptive and unique within the system.
     */
    virtual std::string get_peripheral_name() const = 0;

    /**
     * @brief Checks if the peripheral is available on the current hardware.
     *
     * Determines whether the peripheral can be tested on the current system.
     * This allows the testing framework to skip unsupported peripherals.
     *
     * @return true if the peripheral is available and testable, false otherwise.
     *
     * @note This method should perform minimal checks to avoid side effects.
     */
    virtual bool is_available() const = 0;

protected:
    /**
     * @brief Protected constructor to prevent direct instantiation.
     *
     * The base class constructor is protected to ensure that only derived
     * classes can be instantiated. This enforces the abstract nature of
     * the interface.
     */
    PeripheralTester() = default;

    /**
     * @brief Creates a standardized test report.
     *
     * Helper method for derived classes to create TestReport objects
     * with consistent formatting and metadata.
     *
     * @param result The outcome of the test.
     * @param details Additional information about the test execution.
     * @param test_duration How long the test took to execute.
     *
     * @return TestReport populated with the provided information.
     */
    TestReport create_report(TestResult result, const std::string& details,
                           std::chrono::milliseconds test_duration) const {
        TestReport report;
        report.result = result;
        report.peripheral_name = get_peripheral_name();
        report.duration = test_duration;
        report.details = details;
        report.timestamp = std::chrono::system_clock::now();
        return report;
    }
};

} // namespace cm5_peripheral_test

#endif // PERIPHERAL_TESTER_H