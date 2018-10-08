#include <catch.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include <expression_evaluator/evaluator.hpp>

TEST_CASE("Basic examples", "[Expression evaluator]")
{
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("2");
        REQUIRE(evalutor.Run() == "2");
    }
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("-2 + 2");
        REQUIRE(evalutor.Run() == "0");
    }
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("2 + 2 * 2");
        REQUIRE(evalutor.Run() == "6");
    }
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("(2 + 2) * 2");
        REQUIRE(evalutor.Run() == "8");
    }
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("(103 + 10) / 2 / (3 - 3)");
        REQUIRE(evalutor.Run() == "Error division by zero");
    }
    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("2(");
        REQUIRE(evalutor.Run() == "Error wrong input");
    }

    {
        AsioCalc::Evaluator<int, boost::multiprecision::cpp_dec_float_100> evalutor("(2( + 2))");
        REQUIRE(evalutor.Run() == ") expected");
    }

    {
        int a = std::numeric_limits<int>::max();
        long long expected_value = (long long)a + (long long)a;
        std::string aStr = std::to_string(a);
        std::string expression = "(" + aStr + "+" + aStr + ")";

        AsioCalc::Evaluator<int, long long> evalutor(std::move(expression));
        REQUIRE(evalutor.Run() == std::to_string(expected_value));
    }

    {
        int a = std::numeric_limits<int>::max();
        int expected_value = a + a;
        std::string aStr = std::to_string(a);
        std::string expression = "(" + aStr + "+" + aStr + ")";

        AsioCalc::Evaluator<int, int> evalutor(std::move(expression));
        REQUIRE(evalutor.Run() == std::to_string(expected_value));
    }

    {
        AsioCalc::Evaluator<int, long double> evalutor("1 / 2");
        REQUIRE(evalutor.Run() == "0.5");
    }
}
