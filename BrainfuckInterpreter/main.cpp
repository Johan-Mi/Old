#include <fstream>
#include <iostream>
#include <deque>

int main() {
	std::string program;
	std::string fileBuffer{};

	std::ifstream fileStream("program.txt", std::ios::in);
	while(!fileStream.eof()) {
		fileStream >> fileBuffer;
		program += fileBuffer;
	}

	std::deque<int> memory{0};
	size_t memPointer = 0;
	size_t instPointer = 0;

	while(instPointer >= 0 && instPointer < program.size()) {
		switch(program[instPointer]) {
		case '+':	memory[memPointer]++;	break;
		case '-':	memory[memPointer]--;	break;
		case '>':
			if(++memPointer == memory.size())
				memory.push_back(0);
			break;
		case '<':
			if(memPointer == 0)
				memory.push_front(0);
			else
				memPointer--;
			break;
		case '.':	std::cout << (char)memory[memPointer];	break;
		case ',':
			std::cout << '\n';
			char bufferChar;
			std::cin >> bufferChar;
			memory[memPointer] = bufferChar;
			break;
		case ']':
			if(memory[memPointer] > 0) {
				int iBrackets = 0;
				do {
					if(instPointer < 0) {
						std::cout << "\nError: matching left bracket not found\n";
						return 0;
					}
					(program[instPointer] == '[') && iBrackets++;
					(program[instPointer] == ']') && iBrackets--;
					instPointer--;
				} while(iBrackets != 0);
			}
		}

		instPointer++;
	}

	return 0;
}