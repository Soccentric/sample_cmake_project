include(GoogleTest)

# Original sample tests
add_executable(sample_cmake_project_tests test_sample_cmake_project.cpp)
target_link_libraries(sample_cmake_project_tests PRIVATE sample_cmake_project gtest_main)
target_compile_features(sample_cmake_project_tests PRIVATE cxx_std_17)

if(ENABLE_COVERAGE)
  target_compile_options(sample_cmake_project_tests PRIVATE --coverage)
  target_link_options(sample_cmake_project_tests PRIVATE --coverage)
endif()

gtest_discover_tests(sample_cmake_project_tests)

# GPIO tests
add_subdirectory(gpio)