cmake_minimum_required(VERSION 3.20)
project(AdvancedCalculator VERSION 1.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for M-series optimization
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8.5-a+fp16+sve -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Find required frameworks
find_library(METAL_FRAMEWORK Metal REQUIRED)
find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)
find_library(ACCELERATE_FRAMEWORK Accelerate REQUIRED)

message(STATUS "Metal Framework: ${METAL_FRAMEWORK}")
message(STATUS "Accelerate Framework: ${ACCELERATE_FRAMEWORK}")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/metal-cpp
)

# Source files
set(CALCULATOR_SOURCES
    src/main.cpp
    src/computation/standard_calculator.cpp
    src/computation/complex_calculator.cpp
    src/computation/matrix_calculator.cpp
    src/parser/tokenizer.cpp
    src/parser/expression_parser.cpp
    src/acceleration/metal_interface.cpp
    src/ui/calculator_ui.cpp
)

# Header files
set(CALCULATOR_HEADERS
    include/computation/standard_calculator.hpp
    include/computation/complex_calculator.hpp
    include/computation/matrix_calculator.hpp
    include/parser/tokenizer.hpp
    include/parser/expression_parser.hpp
    include/acceleration/metal_interface.hpp
    include/ui/calculator_ui.hpp
)

# Main executable
add_executable(calculator ${CALCULATOR_SOURCES} ${CALCULATOR_HEADERS})

# Link frameworks
target_link_libraries(calculator
    ${METAL_FRAMEWORK}
    ${FOUNDATION_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
    ${ACCELERATE_FRAMEWORK}
)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS calculator DESTINATION bin)

print_target_properties(calculator)
