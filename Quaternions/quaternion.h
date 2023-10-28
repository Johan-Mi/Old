#pragma once

struct Quaternion {
    float real = 0, i = 0, j = 0, k = 0;

    Quaternion() {}
    Quaternion(const float real) : real(real) {}
    Quaternion(const float real, const float i) : real(real), i(i) {}
    Quaternion(const float real, const float i, const float j)
        : real(real), i(i), j(j) {}
    Quaternion(const float real, const float i, const float j, const float k)
        : real(real), i(i), j(j), k(k) {}

    operator float() {
        return real;
    }

    Quaternion &operator=(const Quaternion &right) {
        real = right.real;
        i = right.i;
        j = right.j;
        k = right.k;
        return *this;
    }
    Quaternion &operator+=(const Quaternion &right) {
        real += right.real;
        i += right.i;
        j += right.j;
        k += right.k;
        return *this;
    }
    Quaternion &operator-=(const Quaternion &right) {
        real -= right.real;
        i -= right.i;
        j -= right.j;
        k -= right.k;
        return *this;
    }
    Quaternion &operator*=(const Quaternion &right) {
        return operator=(*this *right);
    }
    Quaternion &operator/=(const Quaternion &right) {
        return operator=(*this / right);
    }
};

float Norm(const Quaternion &q) {
    return q.real * q.real + q.i * q.i + q.j * q.j + q.k * q.k;
}

Quaternion operator/(const Quaternion &left, float right) {
    return Quaternion(
        left.real / right, left.i / right, left.j / right, left.k / right
    );
}

Quaternion operator+(const Quaternion &left, const Quaternion &right) {
    return Quaternion(
        left.real + right.real, left.i + right.i, left.j + right.j,
        left.k + right.k
    );
}
Quaternion operator-(const Quaternion &left, const Quaternion &right) {
    return Quaternion(
        left.real - right.real, left.i - right.i, left.j - right.j,
        left.k - right.k
    );
}
Quaternion operator*(const Quaternion &q, const Quaternion &r) {
    return Quaternion(
        r.real * q.real - r.i * q.i - r.j * q.j - r.k * q.k,
        r.real * q.i + r.i * q.real - r.j * q.k + r.k * q.j,
        r.real * q.j + r.i * q.k + r.j * q.real + r.k * q.i,
        r.real * q.k - r.i * q.j + r.j * q.i + r.k * q.real
    );
}
Quaternion operator/(const Quaternion &q, const Quaternion &r) {
    return Quaternion(
               (r.real * q.real + r.i * q.i + r.j * q.j + r.k * q.k
               ), /// (r.real * r.real + r.i * r.i + r.j * r.j + r.k * r.k),
               (r.real * q.i - r.i * q.real - r.j * q.k + r.k * q.j
               ), /// (r.real * r.real + r.i * r.i + r.j * r.j + r.k * r.k),
               (r.real * q.j + r.i * q.k - r.j * q.real - r.k * q.i
               ), /// (r.real * r.real + r.i * r.i + r.j * r.j + r.k * r.k),
               (r.real * q.k - r.i * q.j + r.j * q.i - r.k * q.real
               ) /// (r.real * r.real + r.i * r.i + r.j * r.j + r.k * r.k)
           ) /
           Norm(r);
}
