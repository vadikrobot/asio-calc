#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <limits>
#include <bitset>
#include <ctime>
#include <spdlog/spdlog.h>
#include <expression_evaluator/evaluator.hpp>
#include <expression_generator/expression_generator.h>
#include <server/server.h>

std::string ProcessRequest(std::istream &request) {
    std::string str, response;
    bool first{true};
    while(std::getline(request, str)) {
        if (!first) {
            response+="\n";
        }
        first = false;
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evaluator(std::move(str));
        response+= evaluator.Run();
    }
    return response;
}

int main() {
    unsigned short port_num = 8080;
    auto logger = spdlog::stdout_color_mt("main");
    try {
        boost::asio::io_service ios_service;
        AsioCalc::Server srv(port_num, ios_service, ProcessRequest);
        unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;

        std::vector<std::thread> runners;
        for (int i = 0; i < thread_pool_size; i++) {
            runners.emplace_back([&]{ios_service.run();});
        }
        srv.Start();
        for (auto& th : runners) {
            th.join();
        }
    }
    catch (boost::system::system_error& e) {
        const boost::system::error_code& ec = e.code();
        logger->error("Error occured! Error code = {}. Message: {}", ec.value(), ec.message());
    }
    return 0;
}
