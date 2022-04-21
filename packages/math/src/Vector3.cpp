#include "Vector3.h"
#include <cmath>

float Vector3::length() {
	return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3& Vector3::normalize() {
	*this *= 1 / this->length();

	return *this;
}