#include "Matrix4.h"
#include <cmath>

Matrix4::Matrix4(
	float a11, float a21, float a31, float a41,
	float a12, float a22, float a32, float a42,
	float a13, float a23, float a33, float a43,
	float a14, float a24, float a34, float a44
) {
	data[0] = a11;
	data[1] = a21;
	data[2] = a31;
	data[3] = a41;

	data[4] = a12;
	data[5] = a22;
	data[6] = a32;
	data[7] = a42;

	data[8] = a13;
	data[9] = a23;
	data[10] = a33;
	data[11] = a43;

	data[12] = a14;
	data[13] = a24;
	data[14] = a34;
	data[15] = a44;
};

void Matrix4::SetPosition(float x, float y, float z) {
	data[12] = x;
	data[13] = y;
	data[14] = z;
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float far, float near) {
	// solve for x
	float a11 = 2.0f / (right - left);
	float a21 = 0.0f;
	float a31 = 0.0f;
	float a41 = -(left + right) / (right - left);

	// solve for y
	float a12 = 0.0f;
	float a22 = 2.0f / (top - bottom);
	float a32 = 0.0f;
	float a42 = -(top + bottom) / (top - bottom);
	
	// solve for z
	float a13 = 0.0f;
	float a23 = 0.0f;
	float a33 = 2.0f / (near - far);
	float a43 = -(near + far) / (near - far);

	// solve for w
	float a14 = 0.0f;
	float a24 = 0.0f;
	float a34 = 0.0f;
	float a44 = 1.0f;

	return Matrix4(a11, a21, a31, a41, a12, a22, a32, a42, a13, a23, a33, a43, a14, a24, a34, a44);
}

Matrix4 Matrix4::Perspective(float fov, float near, float far) {
	float theta = fov / 2.0f;
	float right = std::tan(fov) * near;
	float top = right; // 1:1 aspect ratio

	// solve for x
	float a11 = near / right;
	float a21 = 0.0f;
	float a31 = 0.0f;
	float a41 = 0.0f;

	// solve for y
	float a12 = 0.0f;
	float a22 = near / top;
	float a32 = 0.0f;
	float a42 = 0.0f;

	// solve for z
	float a13 = 0.0f;
	float a23 = 0.0f;
	float a33 = -(far + near) / (far - near);
	float a43 = -1.0f;

	// solve for w
	float a14 = 0.0f;
	float a24 = 0.0f;
	float a34 = -2.0f * (far * near) / (far - near);
	float a44 = 0.0f;

	return Matrix4(a11, a21, a31, a41, a12, a22, a32, a42, a13, a23, a33, a43, a14, a24, a34, a44);
}
