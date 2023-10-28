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

	std::deque<char> memory{0};
	size_t memPointer = 0;
	size_t instPointer = 0;

	while(instPointer >= 0 && instPointer < program.size()) {
		switch(program[instPointer]) {
			case '+':
				memory[memPointer]++;
				break;
			case '-':
				memory[memPointer]--;
				break;
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
			case '.':
				putchar(memory[memPointer]);
				break;
			case ',':
				putchar('\n');
				memory[memPointer] = getchar();
				break;
			case ']':
				if(memory[memPointer]) {
					int iBrackets = 0;
					do {
						if(instPointer < 0) {
							std::cout << "\nError: matching left bracket not found\n";
							return 0;
						}
						if(program[instPointer] == '[')
							iBrackets++;
						else if(program[instPointer] == ']')
							iBrackets--;
						instPointer--;
					} while(iBrackets);
				}
				break;
		}

		instPointer++;
	}

	return 0;
}
