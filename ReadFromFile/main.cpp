#include <fstream>
#include <vector>
#include <iostream>

int main() {
	std::ifstream fileStream("..\\file.txt", std::ios::in);
	std::vector<std::pair<int, int>> myStack;

	while(!fileStream.eof()) {
		std::pair<int, int> f;
		fileStream >> std::hex >> f.first >> f.second;
		myStack.push_back(f);
		std::cout << f.first << ": " << f.second << '\n';
	}
}