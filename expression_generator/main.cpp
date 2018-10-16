#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include "expression_generator.h"

int main (int argc, char **argv) {
    if (argc < 3) {
        std::cout << "error need 2 params";
        return 1;
    }

    int len = std::atoi(argv[1]);
    int num = std::atoi(argv[2]);

    std::stringstream ss;
    for (int i = 0; i < num; i++) {
        AsioCalc::ExpressionGenerator::generate(len, ss);
        ss << "\n";
    }

    std::cout << ss.rdbuf();
}
