cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Pong VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the subdirectories
add_subdirectory(lib)
add_subdirectory(src)

# Add the executable
add_executable(Pong src/pong.cpp)

# Use target_link_libraries to link the library to our executable
target_link_libraries(Pong PUBLIC GL glut Keyboard pthread)

# Add the binary tree to the search path for include files
target_include_directories(Pong PUBLIC
                           "${PROJECT_BINARY_DIR}")

