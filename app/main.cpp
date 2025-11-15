/**
 * @file main.cpp
 * @brief Main application entry point for sample_cmake_project.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 * 
 * This file contains the main() function that serves as the entry point
 * for the sample_cmake_project application. It demonstrates basic usage
 * of the library API.
 * 
 * @version 1.0
 * @date 2025-11-15
 * 
 * @details
 * The application creates an instance of the main library class and
 * executes its primary functionality. Command-line arguments are currently
 * unused but available for future extensions.
 */

#include "sample_cmake_project/sample_cmake_project.h"
#include <iostream>

/**
 * @brief Main entry point of the application.
 * 
 * Creates an instance of sampleCmakeProject, demonstrates its usage by
 * calling its public methods, and terminates normally.
 * 
 * @param argc Number of command-line arguments (currently unused).
 * @param argv Array of command-line argument strings (currently unused).
 * 
 * @return 0 on successful execution, non-zero on error.
 * 
 * @note Currently, command-line arguments are not processed. Future versions
 *       may add support for configuration via command-line options.
 * 
 * @par Example usage:
 * @code
 * ./my_cmake_project
 * @endcode
 */
int main(int argc, char* argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    // Create an instance of the main library class
    sample_cmake_project::sampleCmakeProject instance("sample_cmake_project");
    
    // Demonstrate basic functionality
    std::cout << "Hello from " << instance.get_name() << std::endl;
    instance.run();

    return 0;
}