#pragma once
#include <math.h>

namespace fb {

	class Vec3 {
	public:

		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vec3 operator+(const Vec3& other) {
			Vec3 v(this->x + other.x, this->y + other.y, this->z + other.z);
			return v;
		}
		Vec3 operator-(const Vec3& other) {
			Vec3 v(this->x - other.x, this->y - other.y, this->z - other.z);
			return v;
		}
		float dot(const Vec3& other) {
			return(this->x * other.x + this->y * other.y + this->z * other.z);
		}

		float length() {
			return sqrtf(x * x + y * y + z * z);
		}

		Vec3 normalize() {
			float l = length();
			Vec3 v(x / l, y / l, z / l);
			return v;
		}

		float x;
		float y;
		float z;
	};

	class Vec4 {
	public:

		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vec4 operator+(const Vec4& other) {
			Vec4 v(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
			return v;
		}
		Vec4 operator-(const Vec4& other) {
			Vec4 v(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
			return v;
		}
		float dot(const Vec4& other) {
			return(this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w);
		}

		float length() {
			return sqrtf(x * x + y * y + z * z);
		}

		Vec4 normalize() {
			float l = length();
			Vec4 v(x / l, y / l, z / l, 1);
			return v;
		}

		float x;
		float y;
		float z;
		float w;
	};

	// 4x4 Matrix, column major. 
	// This means, subsequent elements in the array 
	// form a column in the matrix.
	// The element names denote the row and the column, so
	// m23 means, 2nd row, 3rd column.
	// The counting is 1-based. 
	class Mat4 {
	public: 
		Mat4(float* values) : rawValues(values) 
		{
			m11 = values[0];
			m21 = values[1];
			m31 = values[2];
			m41 = values[3];

			m12 = values[4];
			m22 = values[5];
			m32 = values[6];
			m42 = values[7];

			m13 = values[8];
			m23 = values[9];
			m33 = values[10];
			m43 = values[11];

			m14 = values[12];
			m24 = values[13];
			m34 = values[14];
			m44 = values[15];
		}
		Vec4 operator*(const Vec4& other) {
			float x = m11 * other.x + m12 * other.y + m13 * other.z + m14 * other.w;
			float y = m21 * other.x + m22 * other.y + m23 * other.z + m24 * other.w;
			float z = m31 * other.x + m32 * other.y + m33 * other.z + m34 * other.w;
			float w = m41 * other.x + m42 * other.y + m43 * other.z + m44 * other.w;
			Vec4 v(x, y, z, w);
			return v;
		}

		float* rawValues;

		float m11;
		float m12;
		float m13;
		float m14;

		float m21;
		float m22;
		float m23;
		float m24;

		float m31;
		float m32;
		float m33;
		float m34;

		float m41;
		float m42;
		float m43;
		float m44;

	};
}
