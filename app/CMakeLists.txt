add_executable(cm5_peripheral_test_app main.cpp)
target_link_libraries(cm5_peripheral_test_app PRIVATE gpio_tester cpu_tester)
target_include_directories(cm5_peripheral_test_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_features(cm5_peripheral_test_app PRIVATE cxx_std_17)

# Install
install(TARGETS cm5_peripheral_test_app
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)