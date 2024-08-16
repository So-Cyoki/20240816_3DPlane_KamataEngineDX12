#include "Enemy.h"

Enemy::~Enemy() { delete _model; }

void Enemy::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_model = Model::CreateFromOBJ("Enemy", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	//_worldTransform.rotation_.y = 180 * acosf(-1) / 180;
}

void Enemy::Update() {
	_worldTransform.UpdateMatrix(); // 行列計算
}

void Enemy::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
