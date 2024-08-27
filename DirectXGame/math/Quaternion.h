#pragma once
#include "Vector3.h"
#include <cmath>

struct Quaternion final {
	float w, x, y, z;

public:
	// 数式
	Vector3 operator*(const Vector3& v) const {
		Vector3 u(x, y, z);
		Vector3 cross1 = Vector3::Cross(u, v);
		Vector3 cross2 = Vector3::Cross(u, cross1);
		return v + cross1 * (2.0f * w) + cross2 * 2.0f;
	}
	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z, w * q.x + x * q.w + y * q.z - z * q.y, w * q.y - x * q.z + y * q.w + z * q.x, w * q.z + x * q.y - y * q.x + z * q.w);
	}

	// 归一化四元数
	Quaternion normalize() const {
		float mag = std::sqrt(w * w + x * x + y * y + z * z);
		return Quaternion(w / mag, x / mag, y / mag, z / mag);
	}
	// 将本地旋转变为世界旋转
	Vector3 RotateVector(const Vector3& v) const {
		Quaternion qv(0, v.x, v.y, v.z);
		Quaternion q_conjugate = {w, -x, -y, -z};
		Quaternion result = (*this) * qv * q_conjugate;
		return Vector3(result.x, result.y, result.z);
	}

	// Radian to Quaternion(x,y,z)
	static Quaternion RadianToQuaternion(Vector3 rotate) {
		float rx = rotate.x, ry = rotate.y, rz = rotate.z;
		Quaternion qx(cosf(rx / 2), sinf(rx / 2), 0, 0);
		Quaternion qy(cosf(ry / 2), 0, sinf(ry / 2), 0);
		Quaternion qz(cosf(rz / 2), 0, 0, sinf(rz / 2));
		return qx * qy * qz;
	};
};