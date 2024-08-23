#pragma once
#include "Bullet.h"
#include "Input.h"
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "WorldTransform.h"
#include <algorithm>
#include <cmath>
class Player {
private:
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	Sphere _sphere{};
	float _radius = 5.f;
	Vector3 _pos{};
	Vector3 _rotate{};
	float _moveMaxLength = 900; // 最大移动距离

	// 物理移动
	float _moveSpeed = 0.002f; // 每次增加的速度
	float _rotationSpeed = 0.3f * 0.0001f;
	float _moveBrakeSpeed = 0.05f; // 相对静止速度，刹车（0~1）
	float _moveMax = 1.5f;
	float _moveGasPedal = 0; // 用这个来控制油门的速度
	float _moveGasMax = 0.1f;
	Vector3 _velocity{};
	Vector3 _accelerations{};

	// 战斗属性
	int _attackTime = 10;

	// 瞄准点
	float _screenWidth = float(WinApp::kWindowWidth), _screenHeight = float(WinApp::kWindowHeight);
	Vector2 _arrowMove = {_screenWidth / 2, _screenHeight / 2};  // 代表移动的方向
	Vector2 _arrowMouse = {_screenWidth / 2, _screenHeight / 2}; // 代表鼠标移动的方向

	void Move();
	void ArrowMove();
	void Attack();
	int _currentTimes[31] = {0}; // 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index);

public:
	~Player();
	void Initalize(ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() const { return _worldTransform; };
	const Vector3 GetWorldPosition() const;
	const Sphere& GetSphere() const { return _sphere; };
	const Vector3& GetPos() const { return _pos; };
	const Vector3& GetRotate() const { return _rotate; };
	const Vector3& GetVelocity() const { return _velocity; };
	void SetVelocity(const Vector3& vel) { _velocity = vel; };
	const Vector3& GetAccelerations() const { return _accelerations; };
	const float& GetMoveGasPedal() const { return _moveGasPedal; };
	const float& GetRotationSpeed() const { return _rotationSpeed; };
	const Vector2& GetArrowMove() const { return _arrowMove; };
	const Vector2& GetArrowMouse() const { return _arrowMouse; };
};
