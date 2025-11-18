add_executable(gpio_test_app main.cpp)
target_link_libraries(gpio_test_app PRIVATE gpio_tester)
target_include_directories(gpio_test_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_compile_features(gpio_test_app PRIVATE cxx_std_17)

# Install
install(TARGETS gpio_test_app
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)