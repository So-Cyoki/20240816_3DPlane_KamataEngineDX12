#pragma once
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class CameraController {
private:
	ViewProjection _viewProjection; // 自身の位置(カメラだから、ViewProjectionにしないと)
	Vector3 _cameraPos = {0, 20, -100};
	Vector3 _cameraRotate = {0.4f, 0, 0};
	Vector3 _targetPos{};                // 目標の位置
	Vector3 _targetRotate{};             // 目標の回転
	Vector3 _targetOffset = {0, 7, -20}; // カメラとターゲットの距離の差
	Player* _target = nullptr;

public:
	void Initialize() { _viewProjection.Initialize(); };
	void Update();
	void SetTarget(Player* target) { _target = target; };

	const ViewProjection& GetViewProjection() { return _viewProjection; };
	const Vector3& GetCameraPos() { return _cameraPos; };
	const Vector3& GetCameraRotate() { return _cameraRotate; };
};
