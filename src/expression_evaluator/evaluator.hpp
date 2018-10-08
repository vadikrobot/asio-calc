#include <memory>
#include <bitset>
#include <limits>
#include <boost/multiprecision/cpp_dec_float.hpp>

namespace spdlog {
class logger;
}

namespace AsioCalc {
enum class TokenType {
    number,
    operation,
    final
};

template<class InputInteger, class ResultType>
class Evaluator {
    std::shared_ptr<spdlog::logger> logger;

    class Token {
        Token() = default;
    public:
        TokenType tokenType;
        InputInteger num{0};
        char operation{0};
        bool isFinal() const {return tokenType == TokenType::final; }

        static Token BuildFromInteger(InputInteger &&num) {
            Token t;
            t.tokenType = TokenType::number;
            t.num = std::move(num);
            return t;
        }

        static Token BuildFromOperation(char &operation) {
            Token t;
            t.tokenType = TokenType::operation;
            t.operation = std::move(operation);
            return t;
        }

        static Token FinalToken() {
            static Token final_token;
            final_token.tokenType = TokenType::final;
            return final_token;
        }
    };
    Token lastToken{Token::FinalToken()};
    std::bitset<std::numeric_limits<unsigned char>::max()> mask;
public:
    std::string input;
    std::string::iterator it;
    std::string::iterator prev;
    Evaluator(std::string &&input_);

    Token getNextToken();
    void unread();
    ResultType prim();
    ResultType handleAddition();
    ResultType handleMultiplication();
    std::string Run();
};
}
