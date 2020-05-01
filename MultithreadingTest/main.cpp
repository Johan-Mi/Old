#include <iostream>
#include <thread>
int acm1 = 0;
int acm2 = 0;


void sum(int start, int end, int &store) {
	int n = 0;
	for(int i = start; i <= end; i++) {
		n += i;
	}
	store = n;
}

int main() {
	std::thread t1(sum, 1, 3, std::ref(acm1));
	std::thread t2(sum, 5, 7, std::ref(acm2));

	t1.join();
	t2.join();

	std::cout << acm1 << "\n" << acm2 << "\n";
	return 0;
}