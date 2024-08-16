#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Enemy {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
};
