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

	AABB _aabb{};
	Vector3 _size = {6.3f, 4.f, 11.8f};

	Vector3 _playerPos{};
	Vector3 _playerRotate{};

	float _speed = 1;
	float _speedRotate = 0.01f;

	void Move();

public:
	~Player();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() { return _worldTransform; };
	const Vector3 GetWorldPosition();
	const AABB& GetAABB() { return _aabb; };
};
