#pragma once
#include "Bullet.h"
#include "GameUI.h"
#include "Model.h"
#include "My3DTools.h"
#include "Particle.h"
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
	float _radius = 8.f;
	Vector3 _pos{};
	Vector3 _rotate{};
	// 物理移动
	float _speed = 0.005f;
	float _rotateSpeed = 0.001f; //(0~1)用了插值算法
	float _moveMax = 1.5f;
	Vector3 _velocity{};
	Vector3 _accelerations{};
	Quaternion _randomZ{};  // 随机旋转的Z轴
	float _hurtVel = 0.01f; // 受伤受到的力
	// 战斗属性
	float _hp = 0;
	float _attack = 1; // 这个值没用，玩家受伤数值写在了玩家类中
	int _attackTime = 20;
	float _aimingRadian = 15 * std::acosf(-1) / 180; // 和玩家多少夹角时候才射击
	float _aimingLength = 300;                       // 和玩家多少距离才开始射击
	int _fleeTime = 5 * 60;                          // 逃跑时间
	int _pMoveTime = 10;                             // 移动粒子效果时间

	Player* _playerObj = nullptr;
	GameUI* _gameUIObj = nullptr;
	bool _isHurt = false;
	bool _isDead = false;
	int _deadTime = 5 * 60;              // 多久后结束死亡动画（回收敌人）
	int _deadTime_boom = int(0.5f * 60); // 死亡动画，Boom的频率
	int _deadTime_boom2 = int(0.2f * 60);

	// 状态：0追击、1偷袭、2环绕、3逃跑、4死亡
	enum class State { Chase, Raid, Orbit, Flee, Dead };
	State _currentState = State::Chase;

	void Attack();
	void IsCollision();
	// 是否在瞄准目标
	bool IsAimingAtPlayer(float maxAimAngleRadians);

	// 状态机
	// 追击
	// void Enter_Chase();
	// void Exit_Chase();
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
	void Update_Flee();
	bool IsExit_Flee();
	//// 死亡
	// void Enter_Dead();
	// void Exit_Dead();
	// void Update_Dead();
	// bool IsExit_Dead();

	// 工具
	int _currentTimes[10] = {0};                           // 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index, bool first); // 计时器：帧时间、编号、首次是否输出true
	Quaternion RandomZRotation(float maxAngleRadians);     // 按照输入的角度，随机一个Z轴的四元数

public:
	~Enemy();
	void Initalize(ViewProjection* viewProjection, const Vector3& position, Player* playerObj, GameUI* gameUIObj);
	void Update();
	void Draw();
	void Fire();   // 调用这个方法来触发敌人
	void ToDead(); // 死亡方法

	const Vector3 GetWorldPosition() const;
	const Vector3& GetPos() const { return _pos; };
	const Sphere& GetSphere() const { return _sphere; };
	const bool& GetIsDead() const { return _isDead; };
	const float& GetHp() const { return _hp; };
	void SetHp(const float& hp) { _hp = hp; };
	const Vector3& GetVelocity() const { return _velocity; };
	void SetVelocity(const Vector3& vel) { _velocity = vel; };
};

class EnemyManager {
public:
	inline static bool _isStart = false;
	inline static Vector3 _startPos = {0, 50, 300};
	inline static int _enemyMax = 10;
	inline static int _enemyMin = 2;
	inline static int _bornTime = 10 * 60;

	inline static std::vector<Enemy*> _updatePool;
	inline static std::queue<Enemy*> _idlePool;

	static void Updata();
	static void Draw();
	static void EnemyBornSystem(ViewProjection* viewProjection, Player* playerObj, GameUI* gameUIObj); // 敌人生成系统
	inline static int _currentTimes[5] = {0};
	static bool FrameTimeWatch(int frame, int index, bool first); // 计时器：帧时间、编号、首次是否输出true

	// 获取一个对象，并且初始化
	static Enemy* AcquireEnemy(ViewProjection* viewProjection, const Vector3& position, Player* playerObj, GameUI* gameUIObj);
	// 回收一个对象
	static void ReleaseEnemy(Enemy* enemy);
};
