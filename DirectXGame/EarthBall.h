#pragma once
#include "Bullet.h"
#include "Enemy.h"
#include "Model.h"
#include "My3DTools.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class EarthBall {
private:
	Model* _model = nullptr;
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;

	Sphere _sphere{};
	float _radius = 7.f;
	Vector3 _size = {10, 10, 10};
	Player* _playerObj = nullptr;

	Vector3 _rotationValue = {0, -0.005f, 0}; // 地球的自转角度

	void IsCollision();

public:
	~EarthBall();
	void Initialize(ViewProjection* view, Player* playerObj);
	void Update();
	void Draw();

	const Vector3 GetWorldPosition() const;
	const Sphere& GetSphere() const { return _sphere; };
};
