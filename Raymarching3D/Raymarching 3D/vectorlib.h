#pragma once
#include <cmath>
#include <iostream>

namespace vectorlib {
	const float pi = 3.1415926535;
	const float tau = pi * 2;

	struct Vector2 {
	public:
		float x, y;

		friend std::ostream& operator<<(std::ostream& inStream, const Vector2& inVector);

		~Vector2() {};
		Vector2() : x(0), y(0) {};
		Vector2(float inX, float inY) : x(inX), y(inY) {};

		Vector2 operator+(Vector2 rhs) { return Vector2(x + rhs.x, y + rhs.y); }
		Vector2 operator-(Vector2 rhs) { return Vector2(x - rhs.x, y - rhs.y); }
		Vector2 operator*(float rhs) { return Vector2(x * rhs, y * rhs); }
		Vector2 operator/(float rhs) { return Vector2(x / rhs, y / rhs); }
		float   operator%(Vector2 rhs) { return x * rhs.x + y * rhs.y; }
		//The dot product is % because . can't be overloaded

		void operator=(Vector2 rhs) {
			x = rhs.x;
			y = rhs.y;
		}
		void operator+=(Vector2 rhs) {
			x += rhs.x;
			y += rhs.y;
		}
		void operator-=(Vector2 rhs) {
			x -= rhs.x;
			y -= rhs.y;
		}
		void operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
		}
		void operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
		}

		float magnitude() { return sqrt(x * x + y * y); }
		Vector2 unit() { return Vector2(x / magnitude(), y / magnitude()); }
		Vector2 cross(Vector2 inVector) { return Vector2(x * (inVector.x + inVector.y), y * (inVector.x + inVector.y)); }
		float compSum() { return x + y; }
	};

	struct Vector3 {
	public:
		float x, y, z;

		friend std::ostream& operator<<(std::ostream& inStream, const Vector3& inVector);

		~Vector3() {};
		Vector3() : x(0), y(0), z(0) {};
		Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {};

		Vector3 operator+(Vector3 rhs) { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
		Vector3 operator-(Vector3 rhs) { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
		Vector3 operator*(float rhs) { return Vector3(x * rhs, y * rhs, z * rhs); }
		Vector3 operator/(float rhs) { return Vector3(x / rhs, y / rhs, z / rhs); }
		float   operator%(Vector3 rhs) { return x * rhs.x + y * rhs.y + z * rhs.z; }
		//The dot product is % because . can't be overloaded

		void operator=(Vector3 rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}
		void operator+=(Vector3 rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		}
		void operator-=(Vector3 rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		}
		void operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;
		}
		void operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;
		}

		float magnitude() { return sqrt(x * x + y * y + z * z); }
		Vector3 unit() { return Vector3(x / magnitude(), y / magnitude(), z / magnitude()); }
		Vector3 cross(Vector3 inVector) { return Vector3(x * (inVector.x + inVector.y + inVector.z), y * (inVector.x + inVector.y + inVector.z), z * (inVector.x + inVector.y + inVector.z)); }
		float compSum() { return x + y + z; }
	};

	auto vector2FromAngle = [&](float fAngle) { return Vector2(cosf(fAngle), sinf(fAngle)); };
	auto vector3FromAngles = [&](float fPitch, float fYaw) { return Vector3(cosf(fPitch) * cosf(fYaw), sinf(fPitch), cosf(fPitch) * sinf(fYaw)); };
	std::ostream& operator<<(std::ostream & inStream, const Vector2 & inVector) {
		inStream << '(' << inVector.x << ", " << inVector.y << ')';
		return inStream;
	}
	std::ostream& operator<<(std::ostream & inStream, const Vector3 & inVector) {
		inStream << '(' << inVector.x << ", " << inVector.y << ", " << inVector.z << ')';
		return inStream;
	}
}