#pragma once

struct Quaternion {
    float real = 0, i = 0, j = 0, k = 0;

    Quaternion() {}
    Quaternion(float const real) : real(real) {}
    Quaternion(float const real, float const i) : real(real), i(i) {}
    Quaternion(float const real, float const i, float const j)
        : real(real), i(i), j(j) {}
    Quaternion(float const real, float const i, float const j, float const k)
        : real(real), i(i), j(j), k(k) {}

    operator float() {
        return real;
    }

    Quaternion &operator=(Quaternion const &right) {
        real = right.real;
        i = right.i;
        j = right.j;
        k = right.k;
        return *this;
    }
    Quaternion &operator+=(Quaternion const &right) {
        real += right.real;
        i += right.i;
        j += right.j;
        k += right.k;
        return *this;
    }
    Quaternion &operator-=(Quaternion const &right) {
        real -= right.real;
        i -= right.i;
        j -= right.j;
        k -= right.k;
        return *this;
    }
    Quaternion &operator*=(Quaternion const &right) {
        return operator=(*this *right);
    }
    Quaternion &operator/=(Quaternion const &right) {
        return operator=(*this / right);
    }
};

float Norm(Quaternion const &q) {
    return q.real * q.real + q.i * q.i + q.j * q.j + q.k * q.k;
}

Quaternion operator/(Quaternion const &left, float right) {
    return Quaternion(
        left.real / right, left.i / right, left.j / right, left.k / right
    );
}

Quaternion operator+(Quaternion const &left, Quaternion const &right) {
    return Quaternion(
        left.real + right.real, left.i + right.i, left.j + right.j,
        left.k + right.k
    );
}
Quaternion operator-(Quaternion const &left, Quaternion const &right) {
    return Quaternion(
        left.real - right.real, left.i - right.i, left.j - right.j,
        left.k - right.k
    );
}
Quaternion operator*(Quaternion const &q, Quaternion const &r) {
    return Quaternion(
        r.real * q.real - r.i * q.i - r.j * q.j - r.k * q.k,
        r.real * q.i + r.i * q.real - r.j * q.k + r.k * q.j,
        r.real * q.j + r.i * q.k + r.j * q.real + r.k * q.i,
        r.real * q.k - r.i * q.j + r.j * q.i + r.k * q.real
    );
}
Quaternion operator/(Quaternion const &q, Quaternion const &r) {
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
