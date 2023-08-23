cmake_minimum_required(VERSION 3.15)

project(ConsoleCpp17Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify source files directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add header files to the project
set(HEADER_FILES
    ${INCLUDE_DIR}/utils.h
    ${INCLUDE_DIR}/logger.h
)

# Add source files to the project
set(SOURCE_FILES
    ${SOURCE_DIR}/test.cpp
    #${SOURCE_DIR}/logger.cpp
)

# Include Google Test using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

# Set include directories
include_directories(${INCLUDE_DIR})

# Create the executable using the source and header files
add_executable(ConsoleApp ${SOURCE_FILES})

# Link against Google Test library
target_link_libraries(ConsoleApp PRIVATE gtest_main)

# Tests
enable_testing()

# Add the test executable
add_executable(ConsoleAppTests ${SOURCE_DIR}/test.cpp)

# Link the test executable against Google Test library
target_link_libraries(ConsoleAppTests PRIVATE gtest_main)

# Add a test
add_test(NAME MyTest COMMAND ConsoleAppTests)
