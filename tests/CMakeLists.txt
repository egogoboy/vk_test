cmake_minimum_required(VERSION 3.12)
project(MetricsTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_main test_main.cpp)
add_executable(speed_test speed_test.cpp)

target_link_libraries(test_main metrics)
target_link_libraries(speed_test metrics)
