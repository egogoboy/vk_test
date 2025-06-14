cmake_minimum_required(VERSION 3.12)
project(MetricsLib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(metrics STATIC ${SOURCES})
target_include_directories(metrics PUBLIC ${PROJECT_INCLUDE_DIR})

install(TARGETS metrics DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

add_subdirectory(tests)
