#pragma once
#include "Quaternion.h"
#include "Vector3.h"
#include "algorithm"
#include <cmath>
#include <random>

// 球(Vector3 中心、float 半径)
struct Sphere final {
	Vector3 center;
	float radius;
};
// 軸平行境界箱(Vector3 min、Vector3 max)
struct AABB final {
	Vector3 min;
	Vector3 max;
};

class My3dTools {
public:
	// 物理計算
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
		if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
			return true;

		return false;
	};
	static bool IsCollision(const AABB& aabb, const Sphere& sphere) {
		Vector3 closestPoint{};
		closestPoint.x = {std::clamp(sphere.center.x, aabb.min.x, aabb.max.x)};
		closestPoint.y = {std::clamp(sphere.center.y, aabb.min.y, aabb.max.y)};
		closestPoint.z = {std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)};
		float distance = (closestPoint - sphere.center).Length();
		if (distance <= sphere.radius)
			return true;

		return false;
	};
	static bool IsCollision(const Sphere& s1, const Sphere& s2) {
		float distance = (s2.center - s1.center).Length();
		if (distance <= s1.radius + s2.radius)
			return true;
		return false;
	}
	// 方向计算
	static Vector3 GetDirection_front(Vector3 rotate) {
		// Vector3 front{};
		// front.x = sinf(rotate.y) * cosf(rotate.x);
		// front.y = -sinf(rotate.x);
		// front.z = cosf(rotate.y) * cosf(rotate.x);
		// front.Normalize();
		// return front;
		Quaternion q = Quaternion::RadianToQuaternion(rotate);
		Vector3 localFront = {0, 0, 1};
		Vector3 front = q * localFront;
		return front.Normalize();
	};
	static Vector3 GetDirection_up(Vector3 rotate) {
		// Vector3 front{}, right{}, up{};
		// front.x = sinf(rotate.y) * cosf(rotate.x);
		// front.y = -sinf(rotate.x);
		// front.z = cosf(rotate.y) * cosf(rotate.x);
		// front.Normalize();
		// Vector3 worldUp{0, 1, 0};
		// right = Vector3::Cross(front, worldUp);
		// right.Normalize();
		// up = Vector3::Cross(right, front);
		// up.Normalize();
		// return up;
		Quaternion q = Quaternion::RadianToQuaternion(rotate);
		Vector3 localUp = {0, 1, 0};
		Vector3 up = q * localUp;
		return up.Normalize();
	};
	static Vector3 GetDirection_right(Vector3 rotate) {
		// Vector3 front{}, right{};
		// front.x = sinf(rotate.y) * cosf(rotate.x);
		// front.y = -sinf(rotate.x);
		// front.z = cosf(rotate.y) * cosf(rotate.x);
		// front.Normalize();
		// Vector3 worldUp{0, 1, 0};
		// right = Vector3::Cross(front, worldUp);
		// right.Normalize();
		// return right;
		Quaternion q = Quaternion::RadianToQuaternion(rotate);
		Vector3 localRight = {-1, 0, 0};
		Vector3 right = q * localRight;
		return right.Normalize();
	};
	static Vector3 GetDirection_front(Quaternion quaternion) {
		// quaternion = quaternion.normalize();
		// Vector3 localFront = {0, 0, 1};
		// Vector3 front = quaternion * localFront;
		// return front.Normalize();
		quaternion = quaternion.normalize();
		Vector3 localFront = {0, 0, 1};
		Vector3 front = quaternion.RotateVector(localFront);
		return front.Normalize();
	};
	static Vector3 GetDirection_up(Quaternion quaternion) {
		// quaternion = quaternion.normalize();
		// Vector3 localUp = {0, 1, 0};
		// Vector3 up = quaternion * localUp;
		// return up;
		quaternion = quaternion.normalize();
		Vector3 localUp = {0, 1, 0};
		Vector3 up = quaternion.RotateVector(localUp);
		return up.Normalize();
	};
	static Vector3 GetDirection_right(Quaternion quaternion) {
		// quaternion = quaternion.normalize();
		// Vector3 localRight = {-1, 0, 0};
		// Vector3 right = quaternion * localRight;
		// return right;
		quaternion = quaternion.normalize();
		Vector3 localRight = {-1, 0, 0};
		Vector3 right = quaternion.RotateVector(localRight);
		return right.Normalize();
	};
	// ツール
	static AABB GetAABB(float width, float height, float depth, Vector3 worldPos) {
		AABB aabb{};
		aabb.min = {worldPos.x - width / 2, worldPos.y - height / 2, worldPos.z - depth / 2};
		aabb.max = {worldPos.x + width / 2, worldPos.y + height / 2, worldPos.z + depth / 2};
		return aabb;
	};
	static Sphere GetSphere(float radius, Vector3 worldPos) {
		Sphere sphere{};
		sphere.radius = radius;
		sphere.center = worldPos;
		return sphere;
	};
	// 按照范围，生成一个绕轴旋转的随机四元数(X:0,Y:1,Z:2)
	static Quaternion RandomRotation(float maxRadians, int xyz) {
		// 生成一个在 [-maxAngleRadians, maxAngleRadians] 范围内的随机角度
		std::random_device rd;
		std::mt19937 gen(rd());                                                 // 使用随机设备初始化种子
		std::uniform_real_distribution<float> distrib(-maxRadians, maxRadians); // 指定 float 类型的分布
		float randomAngle = distrib(gen);
		switch (xyz) {
		case 0:
			return Quaternion{std::cosf(randomAngle / 2), std::sinf(randomAngle / 2), 0.0f, 0.0f};
			break;
		case 1:
			return Quaternion{
			    std::cosf(randomAngle / 2),
			    0.0f,
			    std::sinf(randomAngle / 2),
			    0.0f,
			};
			break;
		case 2:
			return Quaternion{std::cosf(randomAngle / 2), 0.0f, 0.0f, std::sinf(randomAngle / 2)};
			break;
		}
		return Quaternion{std::cosf(randomAngle / 2), 0.0f, 0.0f, std::sinf(randomAngle / 2)};
	}
};