#pragma once
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class CameraController {
private:
	ViewProjection _viewProjection; // 自身の位置(カメラだから、ViewProjectionにしないと)
	Vector3 _targetPos{};           // 目標の位置
	Player* _target = nullptr;

public:
	Vector3 _cameraPos = {0, 20, -50};
	Vector3 _cameraRotate = {0.4f, 0, 0};
	void Initialize() { _viewProjection.Initialize(); };
	void Update();
	void SetTarget(Player* target) { _target = target; };

	const ViewProjection& GetViewProjection() { return _viewProjection; };
};
