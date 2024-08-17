#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Enemy {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	AABB _aabb{};
	Vector3 _size = {6.3f, 4.f, 11.8f};

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const Vector3 GetWorldPosition();
	const AABB& GetAABB() { return _aabb; };
};
