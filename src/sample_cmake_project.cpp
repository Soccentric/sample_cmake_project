/**
 * @file sample_cmake_project.cpp
 * @brief Implementation file for the sample_cmake_project library.
 * @author Sandesh Ghimire | sandesh@soccentric
 * @copyright (C) Soccentric LLC. All rights reserved.
 * 
 * This file contains the complete implementation of the sampleCmakeProject class
 * and its private implementation class (PIMPL pattern).
 * 
 * @version 1.0
 * @date 2025-11-15
 * 
 * @details
 * The implementation uses the PIMPL (Pointer to Implementation) idiom to hide
 * implementation details and maintain binary compatibility. All private data
 * members and helper functions are encapsulated in the Impl class.
 */

#include "sample_cmake_project/sample_cmake_project.h"
#include <iostream>
#include <utility>

namespace sample_cmake_project {

/**
 * @class sampleCmakeProject::Impl
 * @brief Private implementation class for sampleCmakeProject.
 * 
 * This class contains the actual data members and private implementation
 * details of the sampleCmakeProject class. It is hidden from the public
 * interface to provide ABI stability and reduce compilation dependencies.
 * 
 * @invariant name_ is never modified after construction.
 */
class sampleCmakeProject::Impl {
public:
    /**
     * @brief Constructs the implementation object.
     * 
     * Initializes the implementation with the provided name string using
     * move semantics for efficiency.
     * 
     * @param name The name string to store (moved into name_).
     * 
     * @note The explicit keyword prevents implicit conversions.
     */
    explicit Impl(std::string name) : name_(std::move(name)) {}
    
    /**
     * @brief The name associated with this object.
     * 
     * Stores the name provided during construction. This member is immutable
     * after initialization (should only be accessed, not modified).
     */
    std::string name_;
};

/**
 * @brief Constructs a sampleCmakeProject object.
 * 
 * Creates a new instance by allocating and initializing the private
 * implementation object with the provided name.
 * 
 * @param name The name to associate with this object (moved).
 * 
 * @throws std::bad_alloc if memory allocation for pimpl_ fails.
 * 
 * @note This implementation uses move semantics to avoid unnecessary copies.
 */
sampleCmakeProject::sampleCmakeProject(std::string name)
    : pimpl_(std::make_unique<Impl>(std::move(name))) {
}

/**
 * @brief Destructor for sampleCmakeProject.
 * 
 * Default implementation is sufficient as unique_ptr handles cleanup automatically.
 * This destructor must be defined in the .cpp file (not inline in the header)
 * because the Impl class is incomplete in the header.
 */
sampleCmakeProject::~sampleCmakeProject() = default;

/**
 * @brief Retrieves the name from the implementation.
 * 
 * Returns a copy of the stored name string.
 * 
 * @return A copy of the name string.
 * 
 * @throws std::bad_alloc if string copy fails.
 * 
 * @note This is a const member function that doesn't modify object state.
 */
std::string sampleCmakeProject::get_name() const {
    return pimpl_->name_;
}

/**
 * @brief Executes the main functionality.
 * 
 * Performs the primary operation of this class by outputting the stored
 * name to standard output.
 * 
 * @post Outputs a message to std::cout.
 * 
 * @note This implementation writes to stdout and may fail if output is redirected
 *       to an invalid stream.
 */
void sampleCmakeProject::run() {
    std::cout << "Running sample_cmake_project with name: " << pimpl_->name_ << std::endl;
}

} // namespace sample_cmake_project