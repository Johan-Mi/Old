#include "complex.h"

int main() {
	std::cout << "e^i*pi = " << round(exp(I * consts::pi));
	std::cout << "\nThe absolute value of 1+i = " << abs(1 + I);
	std::cout << "\ni*i = " << I * I;
	std::cout << "\nThe angle of i = " << angle(I);
	std::cout << "\n(1+i)(1-i) = " << (1 + I) * (1 - I);
	std::cout << "\n(1+i)(2+i) = " << (1 + I) * (2 + I);
	std::cout << "\n(1+i)/(1-i) = " << (1 + I) / (1 - I);
	std::cout << "\nsqrt(-1) = " << csqrt(-1);
	std::cout << "\nsqrt(-2) = " << csqrt(-2);
	std::cout << "\nThe conjugate of 1+i = " << conjugate(1 + I);
	std::cout << "\npi^pi = " << pow(consts::pi, consts::pi);
	std::cout << "\npi^(1/pi) = " << pow(consts::pi, 1 / consts::pi);
	return 0;
}
