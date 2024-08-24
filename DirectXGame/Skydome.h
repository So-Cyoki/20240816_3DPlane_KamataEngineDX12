#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// 天球
/// </summary>
class Skydome {
private:
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	float _size = ViewProjection::_kFarZ * 0.49f;

public:
	~Skydome();
	void Initialize(ViewProjection* view);
	void Update();
	void Draw();
};
