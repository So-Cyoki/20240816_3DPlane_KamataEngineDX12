#pragma once
#include "Bullet.h"
#include "Input.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class CameraController {
	// 好像不管怎么调，相机的偏移位置总是反应的不够迅速，总是会感觉比玩家的慢一拍。
	// 先不管这个问题了，有空再修改吧

private:
	ViewProjection _viewProjection; // 自身の位置(カメラだから、ViewProjectionにしないと)
	Quaternion _currentQuaternion = {1, 0, 0, 0};
	Vector3 _beforeRotate = {0, 0, 0};

	Vector3 _pos = {0, 20, -20};
	Vector3 _rotate = {0.4f, 0, 0};

	Vector3 _targetPos{};                 // 目標の位置
	Vector3 _targetRotate{};              // 目標の回転
	Vector3 _posOffset = {0, 4.f, -20.f}; // カメラとターゲットの距離の差
	Vector3 _rotateOffset = {0.2f, 0, 0}; // カメラとターゲットの回転の差
	Player* _target = nullptr;

	float _speed_startAni = 1;
	bool _isStart = false;
	bool _isStartAni = false;

	int _currentTime = 0;
	int _attackTime = 5;
	void Attack(); // 这是从摄像机的方向射出的子弹

public:
	void Initialize();
	void Update();
	void SetTarget(Player* target) { _target = target; };

	const ViewProjection& GetViewProjection() const { return _viewProjection; };
	const Vector3& GetCameraPos() const { return _pos; };
	const Vector3& GetCameraRotate() const { return _rotate; };
	void SetCameraPos(const Vector3& pos) { _pos = pos; };
	void SetCameraRotate(const Vector3& rotate) { _rotate = rotate; };
	const bool& GetIsStartAni() const { return _isStartAni; };
	void SetIsStartAni(const bool& flag) { _isStartAni = flag; };
	const bool& GetIsStart() const { return _isStart; };
};
