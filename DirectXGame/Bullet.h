#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "Particle.h"
#include "Quaternion.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <queue>
#include <vector>

class Bullet {

private:
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = Model::CreateFromOBJ("Bullet", true);
	uint32_t _textureHandle1 = TextureManager::Load("Bullet/Bullet.png");
	uint32_t _textureHandle2 = TextureManager::Load("Bullet/Bullet2.png");
	Quaternion _currentQuaternion = {1, 0, 0, 0}; // 用来计算出四元数，保证旋转是完全没问题的
	Vector3 _beforeRotate = {0, 0, 0};

	Vector3 _pos{};
	Vector3 _rotate{}; // 虽然还保留在这里，但是实际上已经不使用了，因为去用四元数了
	Sphere _sphere{};
	float _radius = 0.3f;
	Vector3 _scale = {0.4f, 0.4f, 0.4f};

	float _speed = 10.f;
	int _lifeTime = 2 * 60; // 最大存活时间

	bool _isDead = false;
	bool _isHurt = false;

	void Move();

	// 工具
	int _currentTimes[5] = {0}; // 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index);

public:
	inline static enum Type { tPlayer, tEnemy } bulletType;
	Type _type = tEnemy;

	~Bullet();
	void Initalize(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Type type);
	void Update();
	void Draw();
	void Fire();   // 调用这个方法来发射出子弹
	void ToDead(); // 死亡方法

	const Vector3 GetWorldPosition() const;
	const Sphere& GetSphere() const { return _sphere; };
	const bool& GetIsDead() const { return _isDead; };
	void SetIsDead(const bool& flag) { _isDead = flag; };
	void SetIsHurt(const bool& flag) {
		_isHurt = flag;
		_isDead = flag;
	};
	const Quaternion& GetQuaternion() const { return _currentQuaternion; };
};

class BulletManager {
public:
	inline static std::vector<Bullet*> _updatePool_player;
	inline static std::vector<Bullet*> _updatePool_enemy;
	inline static std::queue<Bullet*> _idlePool;

	static void Updata();
	static void Draw();

	// 获取一个对象，并且初始化
	static Bullet* AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Bullet::Type type);
	// 回收一个对象
	static void ReleaseBullet(Bullet* bullet);
};
