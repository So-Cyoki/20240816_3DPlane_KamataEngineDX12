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

	Sphere _sphere{};
	float _radius = 5.f;

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const Vector3 GetWorldPosition();
	const Sphere& GetSphere() { return _sphere; };
};
