#pragma once
#include "Bullet.h"
#include "Model.h"
#include "My3DTools.h"
#include "Player.h"
#include "Quaternion.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "algorithm"
#include "cmath"
#include "queue"
#include "vector"
#include <cstdlib>
#include <ctime>

class Enemy {
public:
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;
	Quaternion _currentQuaternion = {1, 0, 0, 0}; // 用来计算出四元数，保证旋转是完全没问题的
	Vector3 _beforeRotate = {0, 0, 0};
	// 碰撞相关
	Sphere _sphere{};
	float _radius = 5.f;
	Vector3 _pos{};
	Vector3 _rotate{};
	// 物理移动
	float _speed = 0.01f;
	float _rotateSpeed = 0.001f; //(0~1)用了插值算法
	float _moveMax = 1.5f;
	Vector3 _velocity{};
	Vector3 _accelerations{};
	Quaternion _randomZ{}; // 随机旋转的Z轴
	// 战斗属性(还没写相关逻辑)
	float _hp = 10;
	float _attack = 1;
	int _attackTime = 20;
	float _aimingRadian = 15 * std::acosf(-1) / 180; // 和玩家多少夹角时候才射击
	float _aimingLength = 300;                       // 和玩家多少距离才开始射击

	Player* _playerObj = nullptr;

	// 状态：0追击、1偷袭、2环绕、3逃跑、4死亡
	enum class State { Chase, Raid, Orbit, Flee, Dead };
	State _currentState = State::Chase;

	void Attack();
	// 是否在瞄准目标
	bool IsAimingAtPlayer(float maxAimAngleRadians);

	// 状态机
	// 追击
	void Enter_Chase();
	void Exit_Chase();
	void Update_Chase();
	bool IsExit_Chase();
	//// 偷袭
	// void Enter_Raid();
	// void Exit_Raid();
	// void Update_Raid();
	// bool IsExit_Raid();
	//// 环绕
	// void Enter_Orbit();
	// void Exit_Orbit();
	// void Update_Orbit();
	// bool IsExit_Orbit();
	//// 逃跑
	// void Enter_Flee();
	// void Exit_Flee();
	// void Update_Flee();
	// bool IsExit_Flee();
	//// 死亡
	// void Enter_Dead();
	// void Exit_Dead();
	// void Update_Dead();
	// bool IsExit_Dead();

	// 工具
	int _currentTimes[31] = {0};                       // 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index);         // 计时器
	Quaternion RandomZRotation(float maxAngleRadians); // 按照输入的角度，随机一个Z轴的四元数

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position, Player* playerObj);
	void Update();
	void Draw();
	void Fire(); // 调用这个方法来触发敌人

	const Vector3 GetWorldPosition() const;
	const Vector3& GetPos() const { return _pos; };
	const Sphere& GetSphere() const { return _sphere; };
};

class EnemyManager {
public:
	inline static std::vector<Enemy*> _updatePool;
	inline static std::queue<Enemy*> _idlePool;

	static void Updata();
	static void Draw();

	// 获取一个对象，并且初始化
	static Enemy* AcquireEnemy(ViewProjection* viewProjection, const Vector3& position, Player* playerObj);
	// 回收一个对象
	static void ReleaseEnemy(Enemy* enemy);
};
