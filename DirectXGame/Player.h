#pragma once
#include "Bullet.h"
#include "Input.h"
#include "Model.h"
#include "My3DTools.h"
#include "Quaternion.h"
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
	Quaternion _currentQuaternion = {1, 0, 0, 0}; // 用来计算出四元数，保证旋转是完全没问题的
	Vector3 _beforeRotate = {0, 0, 0};

	Sphere _sphere{};
	float _radius = 4.f;
	Vector3 _pos{};
	Vector3 _rotate{};
	float _moveMaxLength = ViewProjection::_kFarZ * 0.35f; // 最大移动距离

	// 物理移动
	float _moveSpeed = 0.8f * 0.001f;      // 每次增加的油门
	float _rotationSpeed = 0.7f * 0.0001f; // 拐弯转向速度
	float _adRotationSpeed = 0.1f * 0.1f;  // 控制左右翻转
	float _moveBrakeSpeed = 0.05f;         // 相对静止速度，刹车（0~1）
	float _moveMax = 1.5f;
	float _moveGasPedal = 0; // 用这个来控制油门的速度
	float _moveGasMax = 0.1f;
	Vector3 _velocity{};
	Vector3 _accelerations{};
	float _hurtVel = 0.5f; // 受伤受到的力

	// 战斗属性
	float _hp = 0;
	float _hpMax = 70;
	float _attack = 1;
	int _attackTime = 5;
	float _hurtHp = 1;   // 受伤数值
	int _pMoveTime = 5;  // 移动粒子效果时间
	int _pDeadTime = 30; // 死亡粒子效果时间

	// 瞄准点
	float _screenWidth = float(WinApp::kWindowWidth), _screenHeight = float(WinApp::kWindowHeight);
	Vector2 _screenPoint = {_screenWidth / 2, _screenHeight / 2};
	Vector2 _arrowMove = _screenPoint;  // 代表移动的方向
	Vector2 _arrowMouse = _screenPoint; // 代表鼠标移动的方向
	float _arrowMouseMax = 428.f / 2;   // 最大移动距离

	void Move();
	void ArrowMove();
	void Attack();
	void IsCollision();
	void ToDead();

	bool _isDead = false;

	// 工具
	int _currentTimes[10] = {0};                    // 这个用于计时器的使用
	bool FrameTimeWatch_true(int frame, int index); // 一开始就会输出一次true

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
	const float& GetMoveGasMax() const { return _moveGasMax; };
	const float& GetRotationSpeed() const { return _rotationSpeed; };
	const Vector2& GetArrowMove() const { return _arrowMove; };
	const Vector2& GetArrowMouse() const { return _arrowMouse; };
	const Quaternion& GetQuaternion() const { return _currentQuaternion; };
	const float& GetAttackValue() const { return _attack; };
	const float& GetHp() const { return _hp; };
	const float& GetHpMax() const { return _hpMax; };
	void SetHp(const float& hp) { _hp = hp; };
	const bool& GetIsDead() const { return _isDead; };
};
