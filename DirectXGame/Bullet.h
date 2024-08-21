#pragma once
#include "Model.h"
#include "My3DTools.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <queue>
#include <vector>

class Bullet {
	// 基础属性
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;
	Model* _model = nullptr;

	Vector3 _pos{};
	Vector3 _rotate{};
	Sphere _sphere{};
	float _radius = 0.1f;
	Vector3 _scale = {0.2f, 0.2f, 0.2f};

	float _speed = 1.f;

	void Move();

public:
	~Bullet();
	void Initalize(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate);
	void Update();
	void Draw();

	const Vector3 GetWorldPosition() const;
	const Sphere& GetSphere() const { return _sphere; };
};

class BulletManager {
public:
	inline static std::vector<Bullet*> _updatePool;

	inline static std::queue<Bullet*> _idlePool;

	// static void BulletUpdata(Bullet::Vector2 cameraPos, vector<vector<char>> mapData, float bgHeight, float minMapSize);
	static Bullet* AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate);
	static void ReleaseBullet(Bullet* bullet);

	static void BulletDraw();
};
