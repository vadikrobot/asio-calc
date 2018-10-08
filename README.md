# asio calculator
Simple calculator.

[![Project Status: WIP - Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](http://www.repostatus.org/badges/latest/wip.svg)](http://www.repostatus.org/#wip)


Server accepts arithmetic expressions throught TCP socket.

Arithmetic expression may constist of \+, \-, \*, \/, \(, \) and integer numbers

## How to use this

# To run server

1. Download this project: `git clone --recursive https://github.com/vadikrobot/asio-calc.git`
2. `cd asio-calc`
3. `mkdir build.release`
4. `cmake -DCMAKE_BUILD_TYPE=Release ..`
5. `make -j4`
6. to run server: `./asio-calc -p 8080`
7. server will listen on port 8080
8. to test `netcat localhost 8080 <<< 2+2`

# To run tests
1. Download this project: `git clone --recursive https://github.com/vadikrobot/asio-calc.git`
2. `cd asio-calc`
3. `mkdir build.debug`
4. `cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=True ..`
5. `make -j4`
6. `ctest` or `./asio-calc-test`



C++ project, built on Cmake, Catch and spdlog.