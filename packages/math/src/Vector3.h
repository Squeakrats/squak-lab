#pragma once

class Vector3 {
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

	float length();
	Vector3& normalize();
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline Vector3& operator*=(Vector3& lhs, const float scalar) {
	lhs.x *= scalar;
	lhs.y *= scalar;
	lhs.y *= scalar;

	return lhs;
}

inline Vector3 operator*(const float scalar, const Vector3& rhs) {
	return Vector3(rhs.x * scalar, rhs.y * scalar, rhs.z * scalar);
}

inline Vector3& operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;

	return lhs;
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}