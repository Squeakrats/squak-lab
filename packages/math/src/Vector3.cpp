#include "Vector3.h"
#include <cmath>

float Vector3::length() {
	return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3& Vector3::normalize() {
	*this *= 1 / this->length();

	return *this;
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b) {
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}