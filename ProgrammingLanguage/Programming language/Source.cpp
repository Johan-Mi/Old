#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <array>
#include "Lexer.h"

#define pun(t) *(t*)&

#pragma region Type system
enum struct Type { Bool, Int, Float };

constexpr int TypeRank(const Type type) {
	switch(type) {
	case Type::Bool:	return 1;
	case Type::Float:	return 2;
	case Type::Int:		return 2;
	default:			return 0;	//Will never happen
	}
}

constexpr bool IsIntegral(const Type type) {
	switch(type) {
	case Type::Bool:	return true;
	case Type::Float:	return false;
	case Type::Int:		return true;
	default:			return false;	//Will never happen
	}
}

struct Rvalue;

struct Lvalue {
	Rvalue* value = nullptr;

	Lvalue(Rvalue& rval) : value(&rval) {}
};

struct Rvalue {
	int value = 0;
	Type type;

	Rvalue(const bool inValue) : type(Type::Bool) { value = 0; pun(bool)value = inValue; }
	Rvalue(const int inValue) : type(Type::Int) { value = 0; pun(int)value = inValue; }
	Rvalue(const float inValue) : type(Type::Float) { value = 0; pun(float)value = inValue; }
	Rvalue(const Lvalue& lval) : value(lval.value->value), type(lval.value->type) {}
	Rvalue(const int inValue, Type inType) : value(inValue), type(inType) {}

	Rvalue operator=(const Rvalue& right) {
		value = right.value;
		type = right.type;
	}

	constexpr void PromoteToType(const Type inType) {
		if(type == inType || IsIntegral(type) == IsIntegral(inType) && TypeRank(type) > TypeRank(inType))
			return;

		bool b = 0;
		int i = 0;
		float f = 0;

		switch(type) {
		case Type::Bool:	b = pun(bool)value; f = b;					break;
		case Type::Int:		i = pun(int)value; b = i; f = (float)i;	break;
		case Type::Float:	f = pun(float)value; i = (int)f; b = f;		break;
		}
		type = inType;
		value = 0;
		switch(type) {
		case Type::Bool:	pun(bool)value = b;	break;
		case Type::Int:		pun(int)value = i;	break;
		case Type::Float:	pun(float)value = f;	break;
		}
	}

	std::string FormattedValue() {
		switch(type) {
		case Type::Bool:	return std::to_string(pun(bool)value);
		case Type::Int:		return std::to_string(pun(int)value);
		case Type::Float:	return std::to_string(pun(float)value);
		default:			return "Unknown type. See definition of FormattedValue.";	//Will never happen
		}
	}
};

constexpr void EnsureSameType(Rvalue& left, Rvalue& right) {
	if(IsIntegral(left.type) && !IsIntegral(right.type))
		left.PromoteToType(right.type);
	else if(IsIntegral(right.type) && !IsIntegral(left.type))
		right.PromoteToType(left.type);
	else if(TypeRank(right.type) > TypeRank(left.type))
		left.PromoteToType(right.type);
	else
		right.PromoteToType(left.type);
}

#define MKBOP(Op) \
Rvalue operator Op(Rvalue left, Rvalue right) { \
	EnsureSameType(left, right); \
	switch(right.type) { \
	case Type::Bool:	return Rvalue((pun(bool )left.value) Op (pun(bool )right.value)); \
	case Type::Int:		return Rvalue((pun(int  )left.value) Op (pun(int  )right.value)); \
	case Type::Float:	return Rvalue((pun(float)left.value) Op (pun(float)right.value)); \
	default:			return 0; /*Will never happen*/ \
	} \
}

MKBOP(+)
MKBOP(-)
MKBOP(*)
MKBOP(/ )
MKBOP(|| )
MKBOP(&&)

#define MKUOP(Op) \
Rvalue operator Op(Rvalue rval) { \
	switch(rval.type) { \
	case Type::Bool:	return Rvalue(Op(pun(bool )rval.value)); \
	case Type::Int:		return Rvalue(Op(pun(int  )rval.value)); \
	case Type::Float:	return Rvalue(Op(pun(float)rval.value)); \
	default:			return 0; /*Will never happen*/ \
	} \
}

MKUOP(!)
#pragma endregion

#pragma region Lexer
constexpr std::array<std::string_view, 3>  lexerTokens{"int", "bool", "float"};
constexpr std::array<std::string_view, 13> lexerDelimiters{";", "+", "-", "*", "/", "=", "!", "(", ")", "{", "}", "[", "]"};
constexpr std::array<std::string_view, 1>  lexerSplitters{" "};
constexpr std::string_view				   lexerToggler = "\"";
#pragma endregion

#pragma region Parser
enum struct TokenType { Number, BOperator, UOperator, Declaration, Statement, Assignment, Other };

constexpr std::array<std::string_view, 4> parserBOperators{"+", "-", "*", "/"};
constexpr std::array<std::string_view, 1> parserUOperators{"!"};
constexpr std::array<std::string_view, 3> parserDeclarations{"int", "bool", "float"};

bool IsStringNumber(const std::string& line) {
	char* p;
	strtod(line.c_str(), &p);
	return *p == 0;
}
#pragma endregion


int main() {
#pragma region Read program form file
	std::ifstream fileStream("program.txt", std::ios::in);
	std::string rawProgram;
	char readBuffer;
	while(!fileStream.eof()) {
		fileStream >> std::noskipws >> readBuffer;
		if(readBuffer != '\n')
			rawProgram += readBuffer;
	}
#pragma endregion

#pragma region Lex program
	std::vector<std::string> lexedProgram = Lex(rawProgram, lexerTokens, lexerDelimiters, lexerSplitters, lexerToggler);
	for(auto& a : lexedProgram) {
		std::cout << a << ", ";
	}
	std::cout << '\n';
#pragma endregion

#pragma region Parse program
	std::vector<std::pair<TokenType, std::string>> parsedProgram;

	for(std::string_view token : lexedProgram) {
		if(IsStringNumber(std::string(token))) {
			parsedProgram.emplace_back(TokenType::Number, token);
		} else if(std::find(parserBOperators.begin(), parserBOperators.end(), token) != parserBOperators.end()) {
			parsedProgram.emplace_back(TokenType::BOperator, token);
		} else if(std::find(parserUOperators.begin(), parserUOperators.end(), token) != parserUOperators.end()) {
			parsedProgram.emplace_back(TokenType::UOperator, token);
		} else if(std::find(parserDeclarations.begin(), parserDeclarations.end(), token) != parserDeclarations.end()) {
			parsedProgram.emplace_back(TokenType::Declaration, token);
		} else {
			parsedProgram.emplace_back(TokenType::Other, token);
		}
	}

	for(auto& a : parsedProgram) {
		std::cout << (int)a.first << ": " << a.second << ", ";
	}
	std::cout << '\n';
#pragma endregion

	return 0;
}