#pragma once
#include <iostream>
#include <cmath>

#define I Complex{0,1}

struct Complex {
	float real = 0, imag = 0;

	Complex(float real) : real(real) {}
	Complex(float real, float imag) : real(real), imag(imag) {}

	Complex &operator=(const Complex& rhs) {
		real = rhs.real;
		imag = rhs.imag;
		return *this;
	}
	Complex operator++() {
		return Complex(real++, imag);
	}
	Complex operator--() {
		return Complex(real--, imag);
	}

	friend std::ostream &operator<<(std::ostream&, const Complex&);
};

Complex operator/(const Complex & lhs, const float & rhs) {
	return {lhs.real / rhs, lhs.imag / rhs};
}
Complex operator+(const Complex & lhs, const Complex & rhs) {
	return {lhs.real + rhs.real, lhs.imag + rhs.imag};
}
Complex operator-(const Complex & lhs, const Complex & rhs) {
	return {lhs.real - rhs.real, lhs.imag - rhs.imag};
}
Complex operator*(const Complex & lhs, const Complex & rhs) {
	return {lhs.real * rhs.real - lhs.imag * rhs.imag,
		lhs.real * rhs.imag + lhs.imag * rhs.real};
}
Complex operator/(const Complex & lhs, const Complex & rhs) {
	return lhs * Complex{rhs.real, -rhs.imag} /
	(rhs.real * rhs.real + rhs.imag * rhs.imag);
}
Complex operator==(const Complex & lhs, const Complex & rhs) {
	return lhs.real == rhs.real && lhs.imag == rhs.imag;
}
Complex operator!=(const Complex & lhs, const Complex & rhs) {
	return lhs.real != rhs.real || lhs.imag != rhs.imag;
}

float abs(const Complex & num) {
	return sqrt(num.real * num.real + num.imag * num.imag);
}
float angle(const Complex & num) {
	return atanf(num.imag / num.real);
}
Complex conjugate(const Complex & num) {
	return {num.real, -num.imag};
}
Complex unit(const Complex & num) {
	return num / abs(num);
}
Complex pow(const float& base, const Complex & power) {
	return std::pow(base, power.real)
		* Complex {cosf(power.imag * log(base)), sinf(power.imag * log(base)) };
}
Complex exp(const Complex& num) {
	return exp(num.real)* Complex { cosf(num.imag), sinf(num.imag) };
}
Complex csqrt(const float& num) {
	return num >= 0 ? sqrt(num) : I * sqrt(-num);
}
Complex round(const Complex& num) {
	return {(float)(int)num.real,(float)(int)num.imag};
}
Complex normal(const Complex& num) {
	return {-num.imag, num.real};
}

Complex &operator+=(Complex & lhs, const Complex & rhs) {
	lhs.real += rhs.real;
	lhs.imag += rhs.imag;
	return lhs;
}
Complex &operator-=(Complex & lhs, const Complex & rhs) {
	lhs.real -= rhs.real;
	lhs.imag -= rhs.imag;
	return lhs;
}
Complex &operator*=(Complex & lhs, const Complex & rhs) {
	lhs = lhs * rhs;
	return lhs;
}
Complex &operator/=(Complex & lhs, const Complex & rhs) {
	lhs = lhs / rhs;
	return lhs;
}


std::ostream &operator<<(std::ostream & stream, const Complex & num) {
	if(num.real == 0 && num.imag == 0) {
		stream << 0;
	} else {
		if(num.real != 0) {
			stream << num.real;
			if(num.imag > 0)
				stream << '+';
		}
		if(num.imag != 0) {
			if(num.imag != 1 && num.imag != -1)
				stream << num.imag;
			if(num.imag == -1)
				stream << '-';
			stream << 'i';
		}
	}

	return stream;
}



namespace consts {
	constexpr double pi = 3.141592653589793;
	constexpr double e = 2.718281828459045;
	constexpr double tau = 2 * pi;
}
