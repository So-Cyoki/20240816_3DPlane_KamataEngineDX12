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

	Vector3 _scale = {3000, 3000, 3000};

public:
	~Skydome();
	void Initialize(ViewProjection* view);
	void Update();
	void Draw();
};
