#pragma once
#include <array>
#include "Vector3.h"
#include "Vector4.h"

class Matrix4 {
public:
	float data[16];

	Matrix4() {};
	Matrix4(
		float a11, float a21, float a31, float a41,
		float a12, float a22, float a32, float a42,
		float a13, float a23, float a33, float a43,
		float a14, float a24, float a34, float a44);

	Vector3 GetPosition();
	void SetPosition(Vector3 position);

	float At(size_t row, size_t col) const {
		return this->data[(col - 1) * 4 + (row - 1)];
	}

	static Matrix4 Identity() {
		return Matrix4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	static Matrix4 Perspective(float fov, float near, float far);
	static Matrix4 Orthographic(float left, float right, float bottom, float top, float far, float near);
	static Matrix4 RotateX(float theta);
	static Matrix4 RotateY(float theta);
	static Matrix4 RotateZ(float theta);
	static Matrix4 Translate(Vector3 translation);
	static Matrix4 Create(Vector3 position, Vector3 rotation);
};

Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs);
Vector4 operator*(const Matrix4& lhs, const Vector4& rhs);