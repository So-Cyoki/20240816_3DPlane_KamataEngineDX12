#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <queue>
#include <vector>

class Bullet {

private:
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	Vector3 _pos{};
	Vector3 _rotate{};
	Sphere _sphere{};
	float _radius = 0.1f;
	Vector3 _scale = {0.2f, 0.2f, 0.2f};
	float _moveMaxLength = ViewProjection::_kFarZ * 0.7f; // 最大范围

	float _speed = 10.f;
	bool _isDead = false;

	void Move();
	void ToDead();

public:
	inline static enum Type { tPlayer, tEnemy } bulletType;
	Type _type;

	~Bullet();
	void Initalize(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate, Type type);
	void Update();
	void Draw();
	void Fire(); // 调用这个方法来发射出子弹

	const Vector3 GetWorldPosition() const;
	const Sphere& GetSphere() const { return _sphere; };
	const bool& GetIsDead() const { return _isDead; };
	void SetIsDead(const bool& flag) { _isDead = flag; };
};

class BulletManager {
public:
	inline static std::vector<Bullet*> _updatePool_player;
	inline static std::vector<Bullet*> _updatePool_enemy;
	inline static std::queue<Bullet*> _idlePool;

	static void Updata();
	static void Draw();

	// 获取一个对象，并且初始化
	static Bullet* AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate, Bullet::Type type);
	// 回收一个对象
	static void ReleaseBullet(Bullet* bullet);
};
