cmake_minimum_required(VERSION 3.0.0)
project(RoadToVulkan VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(CTest)
enable_testing()

add_executable(RoadToVulkan src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)


target_link_libraries(RoadToVulkan -lglfw -lvulkan)