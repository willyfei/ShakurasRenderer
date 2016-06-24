#pragma once
#include <array>


#define SHAKURAS_BEGIN namespace shakuras {
#define SHAKURAS_END }


SHAKURAS_BEGIN;


//œÚ¡ø
template<class S>
class Vector4 {
public:
	S x, y, z, w;

public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Vector4(S xx, S, yy, S zz) : x(xx), y(yy), z(zz), w(1.0f) {}
	Vector4(S xx, S, yy, S zz, S ww) : x(xx), y(yy), z(zz), w(ww) {}

public:
	S length() const {
		return ::sqrt(x * x, y * y, z * z);
	}

	void normalize(S len = 1.0f) {
		S cur_len = length();
		if (cur_len != len) {
			S scale = len / cur_len;
			x *= scale;
			y *= scale;
			z *= scale;
			w = 1.0f;
		}
	}

	void projection() {
		if (w != 0.0f) {
			S inv = 1.0f / w;
			x *= inv;
			y *= inv;
			z *= inv;
		}
	}
};


template<class S>
S DotProduct(const Vector4<S>& v1, const Vector4<S>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<class S>
Vector4<S> CrossProduct(const Vector4<S>& v1, const Vector4<S>& v2) {
	Vector4<S> v3;
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	return v3;
}


template<class S>
Vector4<S> operator+(const Vector4<S>& v1, const Vector4<S>& v2) {
	return Vector4<S>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
template<class S>
Vector4<S> operator-(const Vector4<S>& v1, const Vector4<S>& v2) {
	return Vector4<S>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
template<class S>
Vector4<S> operator*(const Vector4<S>& v1, S t) {
	return Vector4<S>(v1.x * t, v1.y * t, v1.z * t);
}
template<class S>
Vector4<S> operator/(const Vector4<S>& v1, S d) {
	return Vector4<S>(v1.x / d, v1.y / d, v1.z / d);
}


//æÿ’Û
template<class S>
class Matrix44 {
public:
	Matrix44() {
		reset();
	}

public:
	Matrix44<S>& zero() {
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0.0f;
		return *this;
	}

	Matrix44<S>& reset() {
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
	}

	Vector4<S> transform(const Vector4<S>& v1) {
		Vector4<S> v2;
		v2.x = v1.x * m[0][0] + v1.y * m[1][0] + v1.z * m[2][0] + v1.w * m[3][0];
		v2.y = v1.x * m[0][1] + v1.y * m[1][1] + v1.z * m[2][1] + v1.w * m[3][1];
		v2.z = v1.x * m[0][2] + v1.y * m[1][2] + v1.z * m[2][2] + v1.w * m[3][2];
		v2.w = v1.x * m[0][3] + v1.y * m[1][3] + v1.z * m[2][3] + v1.w * m[3][3];
		return v2;
	}

	static Matrix44<S> Translate(S x, S y, S z) {
		Matrix44<S> mat;
		mat.m[3][0] = x;
		mat.m[3][1] = y;
		mat.m[3][2] = z;
		return mat;
	}

	static Matrix44<S> Scale(S x, S y, S z) {
		Matrix44<S> mat;
		mat.m[0][0] = x;
		mat.m[1][1] = y;
		mat.m[2][2] = z;
		return mat;
	}

	static Matrix44<S> Rotate(S x, S y, S z, S theta) {
		Matrix44<S> mat;
		S qsin = (S)sin(theta * 0.5f);
		S qcos = (S)cos(theta * 0.5f);
		Vector4<S> vec(x, y, z, 1.0f);
		S w = qcos;
		vec.normalize();
		x = vec.x * qsin;
		y = vec.y * qsin;
		z = vec.z * qsin;
		mat.m[0][0] = 1 - 2 * y * y - 2 * z * z;
		mat.m[1][0] = 2 * x * y - 2 * w * z;
		mat.m[2][0] = 2 * x * z + 2 * w * y;
		mat.m[0][1] = 2 * x * y + 2 * w * z;
		mat.m[1][1] = 1 - 2 * x * x - 2 * z * z;
		mat.m[2][1] = 2 * y * z - 2 * w * x;
		mat.m[0][2] = 2 * x * z - 2 * w * y;
		mat.m[1][2] = 2 * y * z + 2 * w * x;
		mat.m[2][2] = 1 - 2 * x * x - 2 * y * y;
		mat.m[0][3] = mat.m[1][3] = mat.m[2][3] = 0.0f;
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}

	static Matrix44<S> LookAt(const Vector4<S>& eye, const Vector4<S>& at, const Vector4<S>& up) {
		Vector4<S> xaxis, yaxis, zaxis;
		zaxis = at - eye;
		zaxis.normalize();
		xaxis = CrossProduct(up, zaxis);
		xaxis.normalize();
		yaxis = CrossProduct(zaxis, xaxis);

		Matrix44<S> mat;
		mat.m[0][0] = xaxis.x;
		mat.m[1][0] = xaxis.y;
		mat.m[2][0] = xaxis.z;
		mat.m[3][0] = -DotProduct(xaxis, eye);

		mat.m[0][1] = yaxis.x;
		mat.m[1][1] = yaxis.y;
		mat.m[2][1] = yaxis.z;
		mat.m[3][1] = -DotProduct(yaxis, eye);

		mat.m[0][2] = zaxis.x;
		mat.m[1][2] = zaxis.y;
		mat.m[2][2] = zaxis.z;
		mat.m[3][2] = -dotProduct(zaxis, eye);

		mat.m[0][3] = mat.m[1][3] = mat.m[2][3] = 0.0f;
		mat.m[3][3] = 1.0f;

		return mat;
	}

	static Matrix44<S> Perspective(S fovy, S aspect, S zn, S zf) {
		Matrix44<S> mat;
		mat.zero();

		S fax = 1.0f / (S)tan(fovy * 0.5f);
		mat.m[0][0] = (S)(fax / aspect);
		mat.m[1][1] = (S)(fax);
		mat.m[2][2] = zf / (zf - zn);
		mat.m[3][2] = -zn * zf / (zf - zn);
		mat.m[2][3] = 1;

		return mat;
	}

public:
	std::array<std::array<S, 4>, 4> m;
};


template<class S>
Matrix44<S> operator+(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (size_t i = 0; i != 4; i++) {
		for (size_t ii = 0; ii != 4; ii++) {
			m3.m[i][ii] = m1.m[i][ii] + m2.m[i][ii];
		}
	}
}
template<class S>
Matrix44<S> operator-(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (int i = 0; i != 4; i++) {
		for (int ii = 0; ii != 4; ii++) {
			m3.m[i][ii] = m1.m[i][ii] - m2.m[i][ii];
		}
	}
	return m3;
}
template<class S>
Matrix44<S> operator*(const Matrix44<S>& m1, const Matrix44<S>& m2) {
	Matrix44<S> m3;
	for (size_t i = 0; i != 4; i++) {
		for (size_t ii = 0; ii != 4; ii++) {
			m3.m[ii][i] = m1.m[ii][0] * m2.m[0][i] +
				m1.m[ii][1] * m2.m[1][i] +
				m1.m[ii][2] * m2.m[2][i] +
				m1.m[ii][3] * m2.m[3][i];
		}
	}
}
template<class S>
Matrix44<S> operator*(const Matrix44<S>& m1, S t) {
	Matrix44<S> m2;
	for (int i = 0; i != 4; i++) {
		for (int ii = 0; ii != 4; ii++) {
			m2.m[i][ii] = m1[i][ii] * t;
		}
	}
}


typedef Vector4<float> Vector4f;
typedef Matrix44<float> Matrix44f;


#define kGSPI 3.1415926f


//≤Â÷µ
inline float Interp(float v1, float v2, float t) {
	return v1 + (v2 - v1) * t;
}
template<class T>
T Interp(const T& v1, const T& v2, float t) {
	return v1 + (v2 - v1) * t;
}

SHAKURAS_END;