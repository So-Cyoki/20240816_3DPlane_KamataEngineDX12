#include "Bullet.h"

void Bullet::Move() {
	Vector3 front{};
	front.x = sinf(_rotate.y) * cosf(_rotate.x);
	front.y = -sinf(_rotate.x);
	front.z = cosf(_rotate.y) * cosf(_rotate.x);
	front.Normalize();

	_pos += front * _speed;

	// 最大范围限制
	Vector3 preWorldPos = GetWorldPosition();
	if (_moveMaxLength < preWorldPos.Length()) {
		BulletManager::ReleaseBullet(this);
	}
}

void Bullet::ToDead() { BulletManager::ReleaseBullet(this); }

Bullet::~Bullet() { delete _model; }

void Bullet::Initalize(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate, Type type) {
	_model = Model::CreateFromOBJ("Bullet", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_worldTransform.rotation_ = rotate;
	_pos = position;
	_rotate = rotate;
	_worldTransform.scale_ = _scale;

	_type = type;
	_isDead = false;
}

void Bullet::Update() {
	if (_isDead)
		ToDead();
	Move();

	_worldTransform.translation_ = _pos;
	_worldTransform.rotation_ = _rotate;
	_worldTransform.UpdateMatrix(); // 行列計算

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Bullet::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Bullet::Fire() { BulletManager::_updatePool_player.push_back(this); }

const Vector3 Bullet::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

Bullet* BulletManager::AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate, Bullet::Type type) {
	if (_idlePool.empty()) {
		Bullet* bullet = new Bullet();
		bullet->Initalize(viewProjection, position, rotate, type);
		return bullet;
	} else {
		Bullet* bullet = _idlePool.front();
		_idlePool.pop();
		bullet->Initalize(viewProjection, position, rotate, type);
		return bullet;
	}
}

void BulletManager::ReleaseBullet(Bullet* bullet) {
	switch (bullet->_type) {
	case Bullet::tPlayer: {
		auto it = std::find(_updatePool_player.begin(), _updatePool_player.end(), bullet);
		if (it != _updatePool_player.end()) {
			_updatePool_player.erase(it);
		}
		break;
	}
	case Bullet::tEnemy: {
		auto it = std::find(_updatePool_enemy.begin(), _updatePool_enemy.end(), bullet);
		if (it != _updatePool_enemy.end()) {
			_updatePool_enemy.erase(it);
		}
		break;
	}
	}
	_idlePool.push(bullet);
}

void BulletManager::Updata() {
	for (Bullet* it : _updatePool_player) {
		it->Update();
	}
	for (Bullet* it : _updatePool_enemy) {
		it->Update();
	}
}

void BulletManager::Draw() {
	for (Bullet* it : _updatePool_player) {
		it->Draw();
	}
	for (Bullet* it : _updatePool_enemy) {
		it->Draw();
	}
}
