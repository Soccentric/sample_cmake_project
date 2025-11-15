add_executable(sample_cmake_project_app main.cpp)
target_link_libraries(sample_cmake_project_app PRIVATE sample_cmake_project)
target_compile_features(sample_cmake_project_app PRIVATE cxx_std_17)

# Install
install(TARGETS sample_cmake_project_app
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)