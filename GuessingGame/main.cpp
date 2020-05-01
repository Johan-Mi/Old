#include <iostream>
#include <ctime>

int main() {
	srand(time(NULL));
	int nNum = rand() % 1000 + 1;
	int nGuess = -1;
	std::cout << "Guess my number (1 - 1000)\n";
	while (nNum != nGuess) {
		std::cin >> nGuess;
		if (nGuess > nNum)
			std::cout << "Too high\n";
		else if (nGuess < nNum)
			std::cout << "Too low\n";
	}
	std::cout << "Correct\n";
	return 0;
}
