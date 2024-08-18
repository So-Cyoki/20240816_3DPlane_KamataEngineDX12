#pragma once
#include "Input.h"
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Player {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	Sphere _sphere{};
	float _radius = 5.f;

	Vector3 _playerPos{};
	Vector3 _playerRotate{};

	float _speed = 1;
	float _speedRotate = 0.01f;

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
};
