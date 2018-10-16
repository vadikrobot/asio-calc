#include "evaluator.hpp"
#include <string>
#include <sstream>
#include <iterator>
#include <limits>
#include <spdlog/spdlog.h>


namespace AsioCalc {
template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
T ParseInteger(const std::string &s) {
    return std::stoi(s);
}

template<class InputInteger, class ResultType>
std::string Evaluator<InputInteger, ResultType>::Run() {
    try {
        std::stringstream output;
        output << std::setprecision(std::numeric_limits<ResultType>::max_digits10)
               << handleAddition();
        if (it != input.end()) {
            return "Error wrong input";
        }
        return output.str();
    } catch (const std::exception& e) {
        return e.what();
    }
}

template<class InputInteger, class ResultType>
ResultType Evaluator<InputInteger, ResultType>::handleMultiplication() {
    ResultType left = prim();

    while(true) {
        auto tok = getNextToken();
        if (tok.operation == '*') {
            left*= prim();
        } else if (tok.operation == '/') {
            ResultType denom = prim();
            if (denom == 0) {
                throw std::overflow_error("Error division by zero");
            }
            left/= denom;
        } else {
            unread();
            return left;
        }
    }
}

template<class InputInteger, class ResultType>
ResultType Evaluator<InputInteger, ResultType>::handleAddition() {
    ResultType left = handleMultiplication();

    while(true) {
        auto tok = getNextToken();
        if (tok.operation == '+') {
            left+= handleMultiplication();
        } else if (tok.operation == '-') {
            left-= handleMultiplication();
        } else {
            unread();
            return left;
        }
    }
}

template<class InputInteger, class ResultType>
ResultType Evaluator<InputInteger, ResultType>::prim() {
    auto tok = getNextToken();
    if (tok.tokenType == TokenType::number) {
        return tok.num;
    }
    if (tok.operation == '-') {
        return -prim();
    }
    if (tok.operation == '(') {
        auto res = handleAddition();
        tok = getNextToken();
        if (tok.operation == ')') {
            return res;
        }
        throw std::domain_error(") expected");
    }

    throw std::domain_error("'()' or 'number' expected");
}

template<class InputInteger, class ResultType>
void Evaluator<InputInteger, ResultType>::unread() {
    logger->debug("unread");
    it = prev;
}

template<class InputInteger, class ResultType>
typename Evaluator<InputInteger, ResultType>::Token Evaluator<InputInteger, ResultType>::getNextToken() {
    std::string num;
    char symbol{0};
    prev = it;
    while (it != input.end()) {
        symbol = *it;
        if (std::isspace(*it)) {
            ++it;
            continue;
        }
        if (std::isdigit(symbol)) {
            num.push_back(symbol);
        } else {
            break;
        }
        ++it;
    }

    logger->debug("parsed '{}', '{}'", num, symbol);
    if (!num.empty()) {
        return Token::BuildFromInteger(ParseInteger<InputInteger>(num));
    } else if (mask.count(symbol)) {
        ++it;
        return Token::BuildFromOperation(symbol);
    } else if (it != input.end()) {
        throw std::domain_error("unexpected symbol");
    }

    logger->debug("final token");
    return Token::FinalToken();
}

template<class InputInteger, class ResultType>
Evaluator<InputInteger, ResultType>::Evaluator(std::string &&input_) :
    input{std::move(input_)},
    it{input.begin()},
    prev{it} {

    if (!spdlog::get("parser")) {
        logger = spdlog::stdout_color_mt("parser");
    } else {
        logger = spdlog::get("parser");
    }

    for (char i : {'(', ')', '/', '*', '+', '-'}) {
        mask.insert(i);
    }
}
template class Evaluator<int, long double>;
template class Evaluator<int, boost::multiprecision::cpp_dec_float_100>;
template class Evaluator<int, long long>;
template class Evaluator<int, int>;
}
