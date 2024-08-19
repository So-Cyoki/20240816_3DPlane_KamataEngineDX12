#pragma once
#include "Input.h"
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <algorithm>
#include <cmath>
class Player {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	Sphere _sphere{};
	float _radius = 5.f;

	Vector3 _playerPos{};
	Vector3 _playerRotate{};

	// 现在需要弄一个物理系统
	float _moveSpeed = 0.01f; // 每次增加的速度
	float _rotationSpeed = 0.8f;
	float _moveMax = 10;
	float _gass = 10;
	Vector3 _velocity{};
	Vector3 _accelerations{};

	Vector2 preMousePos{}; // 鼠标前一帧的位置，用以计算鼠标对方向的移动
	void Move();

public:
	~Player();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() { return _worldTransform; };
	const Vector3 GetWorldPosition();
	const Sphere& GetSphere() { return _sphere; };
	const Vector3& GetPos() { return _playerPos; };
	const Vector3& GetRotate() { return _playerRotate; };
	const Vector3& GetVelocity() { return _velocity; };
	const Vector3& GetAccelerations() { return _accelerations; };
};
