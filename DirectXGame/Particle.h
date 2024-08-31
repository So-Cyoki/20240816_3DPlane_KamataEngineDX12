#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "Quaternion.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cmath>
#include <queue>
#include <vector>

class Particle {
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = Model::CreateSphere(4, 4);

	Vector3 _pos{};
	Quaternion _currentQuaternion = {1, 0, 0, 0};
	Vector3 _scale = {1, 1, 1};
	Vector3 _targetScale = {1, 1, 1}; // 想要改变大小调整这个
	uint32_t _textureHandle_hurt = TextureManager::Load("Sprites/particle_hurt.png");
	uint32_t _textureHandle_boom = TextureManager::Load("Sprites/particle_boom.png");
	Vector3 _moveDir{};

	float _speed = 0.0f;
	int _lifeTime = 0; // 最大存活时间

	bool _isDead = false;

	void Move();

	// 工具
	int _currentTimes[5] = {0};                // 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index); // 计时到了才会输出true
	float EaseOutQuart(float t) { return 1 - powf(1 - t, 4); };
	float EaseOutBack(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;
		return 1 + c3 * powf(t - 1, 3) + c1 * powf(t - 1, 2);
	};

public:
	inline static enum Type { tHurt, tBoom, tBoom2 } particleType;
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
	static void ADD_Boom(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate);
	static void ADD_Boom2(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate);

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
