#pragma once
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
class CameraController {
	// 好像不管怎么调，相机的偏移位置总是反应的不够迅速，总是会感觉比玩家的慢一拍。
	// 先不管这个问题了，有空再修改吧

private:
	ViewProjection _viewProjection; // 自身の位置(カメラだから、ViewProjectionにしないと)
	Quaternion _currentQuaternion = {1, 0, 0, 0};

	Vector3 _pos = {0, 20, -20};

	Vector3 _targetPos{}; // 目標の位置
	Player* _target = nullptr;

	Vector3 _posOffset_1rd = {0, 4, 10};
	Quaternion _rotateOffset_1rd = {1, 0, 0, 0};
	Vector3 _posOffset_3rd = {0, 4.f, -20.f};
	Quaternion _rotateOffset_3rd = {1, 0.1f, 0, 0};
	Vector3 _posOffset = _posOffset_3rd;          // カメラとターゲットの距離の差
	Quaternion _rotateOffset = _rotateOffset_3rd; // カメラとターゲットの回転の差
	bool _is1rd = false;

	float _speed_startAni = 1;
	bool _isStart = false;
	bool _isStartAni = false;

public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { _target = target; };

	const ViewProjection& GetViewProjection() const { return _viewProjection; };
	const Vector3& GetCameraPos() const { return _pos; };
	void SetCameraPos(const Vector3& pos) { _pos = pos; };
	const bool& GetIsStartAni() const { return _isStartAni; };
	void SetIsStartAni(const bool& flag) { _isStartAni = flag; };
	const bool& GetIsStart() const { return _isStart; };
};
