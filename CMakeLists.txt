cmake_minimum_required(VERSION 3.0.0)
project(RoadToVulkan VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(RoadToVulkan src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

configure_file(include/Version.h Version.h)
target_include_directories(RoadToVulkan PUBLIC "${PROJECT_BINARY_DIR}")
