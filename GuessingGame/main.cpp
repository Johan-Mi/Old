#include <ctime>
#include <iostream>

int main() {
    srand(time(NULL));
    int num = rand() % 1000 + 1;
    int guess;
    puts("Guess my number (1 - 1000)");
    do {
        std::cin >> guess;
        if (guess > num) {
            puts("Too high");
        } else if (guess < num) {
            puts("Too low");
        }
    } while (num != guess);
    puts("Correct");
    return 0;
}
