cmake_minimum_required(VERSION 3.1)
project(expression-generator)

# Enable C++14
set (CMAKE_CXX_STANDARD 14)

##################################

add_executable(expression-generator main.cpp expression_generator.h expression_generator.cpp)
