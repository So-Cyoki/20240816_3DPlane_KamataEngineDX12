#pragma once
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class CameraController {
	// 好像不管怎么调，相机的偏移位置总是反应的不够迅速，总是会感觉比玩家的慢一拍。
	// 先不管这个问题了，有空再修改吧

public:
	ViewProjection _viewProjection; // 自身の位置(カメラだから、ViewProjectionにしないと)
	Quaternion _currentQuaternion = {1, 0, 0, 0};
	Vector3 _beforeRotate = {0, 0, 0};

	Vector3 _pos = {0, 20, -170};
	Vector3 _rotate = {0.4f, 0, 0};

	Vector3 _targetPos{};                 // 目標の位置
	Vector3 _targetRotate{};              // 目標の回転
	Vector3 _posOffset = {0, 4.f, -15.f}; // カメラとターゲットの距離の差
	Vector3 _rotateOffset = {0.2f, 0, 0}; // カメラとターゲットの回転の差
	Player* _target = nullptr;

public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { _target = target; };

	const ViewProjection& GetViewProjection() const { return _viewProjection; };
	const Vector3& GetCameraPos() const { return _pos; };
	const Vector3& GetCameraRotate() const { return _rotate; };
};
