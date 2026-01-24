cmake_minimum_required(VERSION 3.15)
project(QuantumSimulator VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

# Find Eigen
find_package(Eigen3 REQUIRED NO_MODULE)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)

# Source files
set(SOURCES
    src/core/statevector.cpp
    src/gates/gates.cpp
    src/circuits/circuit.cpp
)

# Create library
add_library(qsim STATIC ${SOURCES})
target_link_libraries(qsim Eigen3::Eigen)

# Examples
add_executable(basic_example examples/basic_example.cpp)
target_link_libraries(basic_example qsim)

add_executable(bell_state examples/bell_state.cpp)
target_link_libraries(bell_state qsim)

# Tests (optional)
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest)
    if(GTest_FOUND)
        add_executable(run_tests tests/test_gates.cpp)
        target_link_libraries(run_tests qsim GTest::GTest GTest::Main)
        add_test(NAME QuantumTests COMMAND run_tests)
    endif()
endif()
