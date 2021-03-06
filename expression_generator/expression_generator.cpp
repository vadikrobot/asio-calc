#include "expression_generator.h"
#include <functional>
#include <random>

namespace AsioCalc {
namespace ExpressionGenerator {
void generate(int lenInSymbol, std::stringstream &ss) {
    enum SymbolType {
        operaion,
        number,
        opened_bracket,
        closed_bracket,
        End
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> symbolGen(0, SymbolType::End - 1);
    std::uniform_int_distribution<> operationGen(0, 3);
    std::uniform_int_distribution<> spacesGen(0, 10);
    std::uniform_int_distribution<> numberGen(1, std::numeric_limits<int>::max());

    int openCloseCounter{0};
    int symbolType = SymbolType::End;
    bool firstSymbol = true;

    bool symbolSet{false};
    std::function<std::string()> genNextToken = [&] {
        while(true) {
            auto nextSymbol = symbolGen(gen);

            if (nextSymbol == SymbolType::operaion) {
                if (firstSymbol || symbolType == SymbolType::operaion || symbolType == SymbolType::opened_bracket) {
                    return genNextToken();
                }
                symbolType = nextSymbol;

                std::vector<std::string> op{"*", "-", "/", "+"};
                int res = operationGen(gen);
                return op[res];
            }
            if (nextSymbol == SymbolType::number) {
                if (symbolType == SymbolType::number || symbolType == SymbolType::closed_bracket) {
                    return genNextToken();
                }
                symbolType = nextSymbol;
                symbolSet = true;
                return std::to_string(numberGen(gen));
            }
            if (nextSymbol == SymbolType::opened_bracket) {
                if (symbolType == SymbolType::number || symbolType == SymbolType::closed_bracket || symbolType == SymbolType::operaion) {
                    return genNextToken();
                }
                symbolType = nextSymbol;
                openCloseCounter++;
                return std::string{"("};
            }
            if (nextSymbol == SymbolType::closed_bracket) {
                if (symbolType == SymbolType::operaion || symbolType == SymbolType::opened_bracket || !openCloseCounter) {
                    return genNextToken();
                }
                symbolType = nextSymbol;
                openCloseCounter--;
                return std::string{")"};
            }

        }
    };

    while(!symbolSet) {
        for (int i = 0; i < lenInSymbol; i++) {
            ss << genNextToken() << std::string(spacesGen(gen), ' ');
            firstSymbol = false;
        }
    }

    if (symbolType == SymbolType::operaion) {
        ss << std::to_string(numberGen(gen));
    }
    while (openCloseCounter--) {
        ss << ")";
    }
}
}
}
