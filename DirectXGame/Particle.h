#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "Quaternion.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <queue>
#include <vector>

class Particle {
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = Model::CreateSphere(2, 2);

	Vector3 _pos{};
	Quaternion _currentQuaternion = {1, 0, 0, 0};
	Vector3 _scale = {1, 1, 1};
	uint32_t _textureHandle = TextureManager::Load("Sprites/particle.png");

	float _speed = 0.1f;
	int _liftTime = 1 * 60; // 最大存活时间

	bool _isDead = false;

	void Move();

	// 工具
	int _currentTimes[5] = {0};                      // 这个用于计时器的使用
	bool FrameTimeWatch_false(int frame, int index); // 计时到了才会输出true

public:
	inline static enum Type { tHurt, tBoom } particleType;
	Type _type = tHurt;

	~Particle() { delete _model; };
	void Initalize(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Type type);
	void Update();
	void Draw();
	void Fire();
	void ToDead();

	const Vector3 GetWorldPosition() const;
	Model* GetModel() const { return _model; };
	const bool& GetIsDead() const { return _isDead; };
	void SetIsDead(const bool& flag) { _isDead = flag; };
};

class ParticleManager {
public:
	// 粒子特效调用方法
	static void ADD_Hurt(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate);

	// 对象池
	inline static std::vector<Particle*> _updatePool;
	inline static std::queue<Particle*> _idlePool;

	static void Updata();
	static void Draw();

	// 获取一个对象，并且初始化
	static Particle* AcquireParticle(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Particle::Type type);
	// 回收一个对象
	static void ReleaseParticle(Particle* particle);
};
