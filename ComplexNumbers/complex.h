#pragma once
#include <cmath>
#include <iostream>

#define I                                                                      \
    Complex {                                                                  \
        0, 1                                                                   \
    }

struct Complex {
    float real = 0, imag = 0;

    Complex(float real) : real(real) {}
    Complex(float real, float imag) : real(real), imag(imag) {}

    Complex &operator=(Complex const &rhs) {
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

    friend std::ostream &operator<<(std::ostream &, Complex const &);
};

Complex operator/(Complex const &lhs, float const &rhs) {
    return {lhs.real / rhs, lhs.imag / rhs};
}
Complex operator+(Complex const &lhs, Complex const &rhs) {
    return {lhs.real + rhs.real, lhs.imag + rhs.imag};
}
Complex operator-(Complex const &lhs, Complex const &rhs) {
    return {lhs.real - rhs.real, lhs.imag - rhs.imag};
}
Complex operator*(Complex const &lhs, Complex const &rhs) {
    return {
        lhs.real * rhs.real - lhs.imag * rhs.imag,
        lhs.real * rhs.imag + lhs.imag * rhs.real};
}
Complex operator/(Complex const &lhs, Complex const &rhs) {
    return lhs * Complex{rhs.real, -rhs.imag} /
           (rhs.real * rhs.real + rhs.imag * rhs.imag);
}
bool operator==(Complex const &lhs, Complex const &rhs) {
    return lhs.real == rhs.real && lhs.imag == rhs.imag;
}
bool operator!=(Complex const &lhs, Complex const &rhs) {
    return lhs.real != rhs.real || lhs.imag != rhs.imag;
}

float abs(Complex const &num) {
    return sqrt(num.real * num.real + num.imag * num.imag);
}
float angle(Complex const &num) {
    return atanf(num.imag / num.real);
}
Complex conjugate(Complex const &num) {
    return {num.real, -num.imag};
}
Complex unit(Complex const &num) {
    return num / abs(num);
}
Complex pow(float const &base, Complex const &power) {
    return std::pow(base, power.real) *
           Complex{cosf(power.imag * log(base)), sinf(power.imag * log(base))};
}
Complex exp(Complex const &num) {
    return exp(num.real) * Complex{cosf(num.imag), sinf(num.imag)};
}
Complex csqrt(float const &num) {
    return num >= 0 ? sqrt(num) : I * sqrt(-num);
}
Complex round(Complex const &num) {
    return {(float)(int)num.real, (float)(int)num.imag};
}
Complex normal(Complex const &num) {
    return {-num.imag, num.real};
}

Complex &operator+=(Complex &lhs, Complex const &rhs) {
    lhs.real += rhs.real;
    lhs.imag += rhs.imag;
    return lhs;
}
Complex &operator-=(Complex &lhs, Complex const &rhs) {
    lhs.real -= rhs.real;
    lhs.imag -= rhs.imag;
    return lhs;
}
Complex &operator*=(Complex &lhs, Complex const &rhs) {
    lhs = lhs * rhs;
    return lhs;
}
Complex &operator/=(Complex &lhs, Complex const &rhs) {
    lhs = lhs / rhs;
    return lhs;
}

std::ostream &operator<<(std::ostream &stream, Complex const &num) {
    if (num.real == 0 && num.imag == 0) {
        stream << 0;
    } else {
        if (num.real != 0) {
            stream << num.real;
            if (num.imag > 0) {
                stream << '+';
            }
        }
        if (num.imag != 0) {
            if (num.imag != 1 && num.imag != -1) {
                stream << num.imag;
            }
            if (num.imag == -1) {
                stream << '-';
            }
            stream << 'i';
        }
    }

    return stream;
}

namespace consts {
constexpr double pi = 3.141592653589793;
constexpr double e = 2.718281828459045;
constexpr double tau = 2 * pi;
} // namespace consts
