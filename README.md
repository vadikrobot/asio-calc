# asio calculator
Simple calculator. C++ project, built on Cmake, Catch and spdlog, boost, boost asio

[![Project Status: WIP - Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](http://www.repostatus.org/badges/latest/wip.svg)](http://www.repostatus.org/#wip)


Server accepts arithmetic expressions throught TCP socket.

Arithmetic expression may constist of \+, \-, \*, \/, \(, \) and integer numbers

## How to use this

# To run server

1. Download this project: `git clone --recursive https://github.com/vadikrobot/asio-calc.git`
2. `cd asio-calc`
3. `mkdir build.release && cd build.release`
4. `cmake -DCMAKE_BUILD_TYPE=Release ..`
5. `make -j4`
6. to run server: `./asio-calc -p 8080`
7. server will listen on port 8080
8. to test `netcat localhost 8080 <<< 2+2`

# To run tests
1. Download this project: `git clone --recursive https://github.com/vadikrobot/asio-calc.git`
2. `cd asio-calc`
3. `mkdir build.debug && cd build.debug`
4. `cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=True ..`
5. `make -j4`
6. `ctest` or `./asio-calc-test`


# Make random expression
1. run server
2. in separate console `cd asio-calc/expression_generator`
3. `mkdir build && cd build`
4. `cmake -DCMAKE_BUILD_TYPE=Release ..`
5. `make -j4`
6. `./expression-generator 100 100000 > /tmp/expr`
7. `netcat localhost 8080 < /tmp/expr`
