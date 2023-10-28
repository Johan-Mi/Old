#include "lexer.h"
#include <experimental/array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <variant>

namespace std {
using namespace experimental::fundamentals_v2;
}

enum struct Type { Bool, Int, Float };

constexpr int TypeRank(const Type type) {
    switch (type) {
    case Type::Bool:
        return 1;
    case Type::Float:
        return 2;
    case Type::Int:
        return 2;
    default:
        return 0;
    }
}

constexpr bool IsIntegral(const Type type) {
    switch (type) {
    case Type::Bool:
        return true;
    case Type::Float:
        return false;
    case Type::Int:
        return true;
    default:
        return false;
    }
}

struct Rvalue;

struct Lvalue {
    Rvalue *value = nullptr;

    Lvalue(Rvalue &rval) : value(&rval) {}
};

struct Rvalue {
    Type type;
    std::variant<bool, float, int> value;

    Rvalue(bool const v) : type(Type::Bool), value(v) {}
    Rvalue(int const v) : type(Type::Int), value(v) {}
    Rvalue(float const v) : type(Type::Float), value(v) {}
    Rvalue(Lvalue const &lval)
        : type(lval.value->type), value(lval.value->value) {}
    Rvalue(int const v, Type inType) : type(inType), value(v) {}

    Rvalue &operator=(Rvalue const &right) {
        value = right.value;
        type = right.type;
        return *this;
    }

    constexpr void PromoteToType(const Type inType) {
        if (type == inType || (IsIntegral(type) == IsIntegral(inType) &&
                               TypeRank(type) > TypeRank(inType))) {
            return;
        }

        bool b = 0;
        int i = 0;
        float f = 0;

        switch (type) {
        case Type::Bool:
            b = std::get<bool>(value);
            f = b;
            break;
        case Type::Int:
            i = std::get<int>(value);
            b = i;
            f = (float)i;
            break;
        case Type::Float:
            f = std::get<float>(value);
            i = (int)f;
            b = f;
            break;
        }
        type = inType;
        value = 0;
        switch (type) {
        case Type::Bool:
            value = b;
            break;
        case Type::Int:
            value = i;
            break;
        case Type::Float:
            value = f;
            break;
        }
    }

    std::string FormattedValue() {
        switch (type) {
        case Type::Bool:
            return std::to_string(std::get<bool>(value));
        case Type::Int:
            return std::to_string(std::get<int>(value));
        case Type::Float:
            return std::to_string(std::get<float>(value));
        default:
            return "Unknown type. See definition of FormattedValue.";
        }
    }
};

constexpr void EnsureSameType(Rvalue &left, Rvalue &right) {
    if (IsIntegral(left.type) && !IsIntegral(right.type)) {
        left.PromoteToType(right.type);
    } else if (IsIntegral(right.type) && !IsIntegral(left.type)) {
        right.PromoteToType(left.type);
    } else if (TypeRank(right.type) > TypeRank(left.type)) {
        left.PromoteToType(right.type);
    } else {
        right.PromoteToType(left.type);
    }
}

#define MKBOP(Op)                                                              \
    Rvalue operator Op(Rvalue left, Rvalue right) {                            \
        EnsureSameType(left, right);                                           \
        switch (right.type) {                                                  \
        case Type::Bool:                                                       \
            return Rvalue((std::get<bool>(left.value)                          \
            )Op(std::get<bool>(right.value)));                                 \
        case Type::Int:                                                        \
            return Rvalue((std::get<int>(left.value)                           \
            )Op(std::get<int>(right.value)));                                  \
        case Type::Float:                                                      \
            return Rvalue((std::get<float>(left.value)                         \
            )Op(std::get<float>(right.value)));                                \
        default:                                                               \
            return 0;                                                          \
        }                                                                      \
    }

MKBOP(+);
MKBOP(-);
MKBOP(*);
MKBOP(/);
MKBOP(||);
MKBOP(&&);

#define MKUOP(Op)                                                              \
    Rvalue operator Op(Rvalue rval) {                                          \
        switch (rval.type) {                                                   \
        case Type::Bool:                                                       \
            return Rvalue(Op(std::get<bool>(rval.value)));                     \
        case Type::Int:                                                        \
            return Rvalue(Op(std::get<int>(rval.value)));                      \
        case Type::Float:                                                      \
            return Rvalue(Op(std::get<float>(rval.value)));                    \
        default:                                                               \
            return 0;                                                          \
        }                                                                      \
    }

MKUOP(!);

constexpr auto lexerTokens =
    std::make_array<std::string_view>("int", "bool", "float");
constexpr auto lexerDelimiters = std::make_array<std::string_view>(
    ";", "+", "-", "*", "/", "=", "!", "(", ")", "{", "}", "[", "]"
);
constexpr auto lexerSplitters = std::make_array<std::string_view>(" ");
constexpr std::string_view lexerToggler = "\"";

enum struct TokenType {
    Number,
    BOperator,
    UOperator,
    Declaration,
    Statement,
    Assignment,
    Other
};

constexpr auto parserBOperators =
    std::make_array<std::string_view>("+", "-", "*", "/");
constexpr auto parserUOperators = std::make_array<std::string_view>("!");
constexpr auto parserDeclarations =
    std::make_array<std::string_view>("int", "bool", "float");

bool IsStringNumber(std::string const &line) {
    char *p;
    strtod(line.c_str(), &p);
    return *p == 0;
}

int main() {
    std::ifstream fileStream("program.txt", std::ios::in);
    std::string rawProgram;
    char readBuffer;
    while (!fileStream.eof()) {
        fileStream >> std::noskipws >> readBuffer;
        if (readBuffer != '\n') {
            rawProgram += readBuffer;
        }
    }

    std::vector<std::string> lexedProgram =
        Lex(rawProgram, lexerTokens, lexerDelimiters, lexerSplitters,
            lexerToggler);
    for (auto &a : lexedProgram) {
        std::cout << a << ", ";
    }
    std::cout << '\n';

    std::vector<std::pair<TokenType, std::string>> parsedProgram;

    for (std::string_view token : lexedProgram) {
        if (IsStringNumber(std::string(token))) {
            parsedProgram.emplace_back(TokenType::Number, token);
        } else if(std::find(parserBOperators.begin(),
					parserBOperators.end(), token) != parserBOperators.end()) {
            parsedProgram.emplace_back(TokenType::BOperator, token);
        } else if(std::find(parserUOperators.begin(),
					parserUOperators.end(), token) != parserUOperators.end()) {
            parsedProgram.emplace_back(TokenType::UOperator, token);
        } else if(std::find(parserDeclarations.begin(),
					parserDeclarations.end(), token)
				!= parserDeclarations.end()) {
            parsedProgram.emplace_back(TokenType::Declaration, token);
        } else {
            parsedProgram.emplace_back(TokenType::Other, token);
        }
    }

    for (auto &a : parsedProgram) {
        std::cout << (int)a.first << ": " << a.second << ", ";
    }
    std::cout << '\n';

    return 0;
}
