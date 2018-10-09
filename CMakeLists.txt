cmake_minimum_required(VERSION 3.1)
project(asio-calc)

set(asio-calc-build_tests OFF CACHE BOOL "Whether to build tests")

# Enable C++14
set (CMAKE_CXX_STANDARD 14)

################################
# Find thread
################################
find_package(Threads REQUIRED)

################################
# Catch
################################
set(Catch_INCLUDE_DIR vendor/Catch/include)

##################################
# Spdlog
##################################
set(spdlog_SOURCE_DIR vendor/spdlog)
include_directories(${spdlog_SOURCE_DIR}/include)

##################################
# boost
##################################
find_package(Boost REQUIRED COMPONENTS thread system program_options)


##################################
include_directories(src)

set(expression_evaluator_SOURCE
    src/expression_evaluator/evaluator.hpp
    src/expression_evaluator/evaluator.cpp
    )

set(server_SOURCE
    src/server/server.h
    src/server/server.cpp
    )

add_library(expression_evaluator STATIC ${expression_evaluator_SOURCE})

add_library(server STATIC ${server_SOURCE})
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

add_executable(asio-calc src/main.cpp)
target_link_libraries(asio-calc expression_evaluator server)
include_directories(asio-calc ${Boost_INCLUDE_DIRS})
link_directories(asio-calc ${Boost_LIBRARY_DIRS})
target_link_libraries(asio-calc ${Boost_LIBRARIES})

if (ENABLE_TESTS)
    message("Testing enabled")
    enable_testing()

    add_executable(asio-calc-test test/example_test.cpp test/test_main.cpp)
    target_include_directories(asio-calc-test PUBLIC ${Catch_INCLUDE_DIR})
    target_link_libraries(asio-calc-test expression_evaluator)
    target_link_libraries(asio-calc-test ${CMAKE_THREAD_LIBS_INIT})

    add_test(expression_evaluator asio-calc-test)
endif()
