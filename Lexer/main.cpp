#include "lexer.h"
#include <iostream>

int main() {
	auto lexed = lex("if(true) print('Hello, world!');", {"if", "true", "print"}, {"(", ")", ";"}, {" "}, "'");
	for(auto printToken : lexed)
		std::cout << printToken << '\n';
}
