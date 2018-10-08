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

int main (int argc, char **argv) {
    static auto logger = spdlog::stdout_color_mt("console");

    int n{1};
    double tot_time{0};

    std::ofstream output1("/tmp/res1");
    std::ofstream output2("/tmp/res2");

    for (int i = 0; i < n; i++) {
        auto expression = AsioCalc::ExpressionGenerator::generate(1024 * 1024);
        auto copy = expression;

        double time1 = clock();
        AsioCalc::Evaluator<int, long double> parser(std::move(copy));
        auto res = parser.Run();
        double time2 = clock();
        double delta = (time2 - time1) / CLOCKS_PER_SEC;
        tot_time+= delta;

        output1 << expression << "\n";
        output2 << res << "\n";

        logger->info("Time {}", delta);
    }

    logger->info("Avg. duration {}", tot_time / n);
}
