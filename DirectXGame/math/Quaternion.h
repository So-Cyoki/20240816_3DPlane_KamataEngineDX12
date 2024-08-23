#pragma once
#include <cmath>

class Quaternion final {
public:
	float w, x, y, z;

public:
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