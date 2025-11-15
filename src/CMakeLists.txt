add_library(sample_cmake_project STATIC)
target_sources(sample_cmake_project
  PRIVATE
    sample_cmake_project.cpp
)
target_include_directories(sample_cmake_project
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(sample_cmake_project PUBLIC cxx_std_17)

# Install
install(TARGETS sample_cmake_project
  EXPORT sample_cmake_projectTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)