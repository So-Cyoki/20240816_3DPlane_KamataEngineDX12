#pragma once
#include "Vector3.h"
#include <cmath>

class Quaternion final {
public:
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

	// Angles to Quaternion(x,y,z)
	static Quaternion AngleToQuaternion(float roll, float pitch, float yaw) {
		Quaternion q{};

		float cy = cosf(yaw * 0.5f);
		float sy = sinf(yaw * 0.5f);
		float cp = cosf(pitch * 0.5f);
		float sp = sinf(pitch * 0.5f);
		float cr = cosf(roll * 0.5f);
		float sr = sinf(roll * 0.5f);

		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	};
};