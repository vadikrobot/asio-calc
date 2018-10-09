#include <spdlog/spdlog.h>
#include <expression_evaluator/evaluator.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

#include <server/server.h>

std::string ProcessRequest(std::istream &request) {
    std::string str;
    std::stringstream response;
    bool first{true};
    while(std::getline(request, str)) {
        if (!first) {
            response << "\n";
        }
        first = false;
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evaluator(std::move(str));
        response << evaluator.Run();
    }
    return response.str();
}

int main(int argc, char **argv) {
    auto logger = spdlog::stdout_color_mt("main");
    unsigned short portNum = 8080;

    boost::program_options::options_description desc("Options");
    desc.add_options()
            ("port,p", boost::program_options::value<int>(), "port")
            ("log_level", boost::program_options::value<std::string>(), "log verbosity; only debug is supported");

    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);
    } catch (...) {
        logger->error("Error occured! Unknown arguments");
    }

    if (vm.count("p")) {
        portNum = vm["p"].as<int>();
    }

    if (vm.count("log_level")) {
        if (vm["log_level"].as<std::string>() == "debug") {
            spdlog::set_level(spdlog::level::trace);
        }
    }

    try {
        boost::asio::io_service ios_service;
        AsioCalc::Server srv(portNum, ios_service, ProcessRequest);
        std::vector<std::thread> runners;
        for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
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
