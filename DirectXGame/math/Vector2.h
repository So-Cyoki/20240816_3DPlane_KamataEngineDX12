#pragma once
#include <cmath>

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

public:
	// 数式
	Vector2& operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	Vector2& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}
	Vector2& operator*=(const int scalar) {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}
	Vector2 operator+(const Vector2 other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 operator-(const Vector2 other) const { return Vector2{x - other.x, y - other.y}; }
	Vector2 operator*(const Vector2 other) const { return Vector2{x * other.x, y * other.y}; }
	Vector2 operator*(const float scalar) const { return Vector2{x * scalar, y * scalar}; }
	Vector2 operator*(const int scalar) const { return Vector2{x * scalar, y * scalar}; }
	friend Vector2 operator*(const float scalar, const Vector2& v) { return Vector2{scalar * v.x, scalar * v.y}; }
	friend Vector2 operator*(const int scalar, const Vector2& v) { return Vector2{scalar * v.x, scalar * v.y}; }

	// 正規化
	Vector2 Normalize() {
		float length = sqrtf(powf(x, 2) + powf(y, 2));
		return Vector2(x / length, y / length);
	}
	// 長さ
	float Length() { return sqrtf(x * x + y * y); }
};
