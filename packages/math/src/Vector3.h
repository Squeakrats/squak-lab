#pragma once

class Vector3 {
public:
	double x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {};
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}