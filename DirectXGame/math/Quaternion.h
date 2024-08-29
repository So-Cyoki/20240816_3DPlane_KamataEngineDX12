#pragma once
#include "Vector3.h"
#include <algorithm>
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
		float mag = std::sqrtf(w * w + x * x + y * y + z * z);
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
	// 望向指定的坐标
	static Quaternion LookAt(Vector3 myPos, Vector3 targetPos) {
		// 1. 计算方向向量
		Vector3 direction = targetPos - myPos;

		// 检查方向向量是否为零向量
		if (direction.Length() == 0) {
			// 如果玩家和敌人位置重合，返回一个默认的旋转四元数（例如不做任何旋转）
			return Quaternion{1.0, 0.0, 0.0, 0.0}; // 默认旋转（没有旋转）
		}

		direction = direction.Normalize();

		// 2. 定义当前前向向量（Z轴正方向）
		Vector3 forward = {0.0f, 0.0f, 1.0f};

		// 3. 计算旋转轴
		Vector3 rotationAxis = Vector3::Cross(forward, direction);

		// 如果 rotationAxis 是零向量，意味着方向向量和前向向量平行或反平行
		if (rotationAxis.Length() == 0) {
			// 如果平行且同方向，返回单位四元数
			if (Vector3::Dot(forward, direction) > 0) {
				return Quaternion{1.0f, 0.0f, 0.0f, 0.0f}; // 没有旋转
			} else {
				// 如果平行且反方向，返回180度旋转四元数（绕任意轴）
				return Quaternion{0.0f, 0.0f, 1.0f, 0.0f}; // 180度旋转
			}
		}

		rotationAxis = rotationAxis.Normalize();

		// 4. 计算旋转角度
		float dotProduct = Vector3::Dot(forward, direction);

		// 修正 dotProduct 使其在 [-1, 1] 之间
		dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

		float angle = std::acosf(dotProduct);

		// 5. 计算旋转四元数
		Quaternion rotationQuaternion{};
		rotationQuaternion.w = std::cosf(angle / 2);
		rotationQuaternion.x = rotationAxis.x * std::sinf(angle / 2);
		rotationQuaternion.y = rotationAxis.y * std::sinf(angle / 2);
		rotationQuaternion.z = rotationAxis.z * std::sinf(angle / 2);

		return rotationQuaternion;
	};
	// Slerp插值算法
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) {
		// 计算四元数点积
		float dotProduct = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

		// 如果点积为负，反转一个四元数以获得最短路径
		if (dotProduct < 0.0f) {
			dotProduct = -dotProduct;
			return Slerp({-q2.w, -q2.x, -q2.y, -q2.z}, q1, t);
		}

		// 如果点积接近 1，直接使用线性插值
		const float threshold = 0.9995f;
		if (dotProduct > threshold) {
			Quaternion result = {q1.w + t * (q2.w - q1.w), q1.x + t * (q2.x - q1.x), q1.y + t * (q2.y - q1.y), q1.z + t * (q2.z - q1.z)};
			return result.normalize();
		}

		// 使用 Slerp 计算
		float theta_0 = std::acosf(dotProduct); // θ
		float theta = theta_0 * t;              // θ

		float sin_theta = std::sinf(theta);
		float sin_theta_0 = std::sinf(theta_0);

		float s0 = std::cosf(theta) - dotProduct * sin_theta / sin_theta_0;
		float s1 = sin_theta / sin_theta_0;

		return {(q1.w * s0 + q2.w * s1), (q1.x * s0 + q2.x * s1), (q1.y * s0 + q2.y * s1), (q1.z * s0 + q2.z * s1)};
	}
};