#include "Bullet.h"

void Bullet::Move() {
	Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);

	_pos += front * _speed;

	// 存活时间限制
	if (FrameTimeWatch(_liftTime, 0))
		BulletManager::ReleaseBullet(this);
}

void Bullet::ToDead() { BulletManager::ReleaseBullet(this); }

bool Bullet::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] <= 0) {
		_currentTimes[index] = frame;
		return true;
	}
	_currentTimes[index]--;
	return false;
}

Bullet::~Bullet() { delete _model; }

void Bullet::Initalize(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Type type) {
	_model = Model::CreateFromOBJ("Bullet", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
	_rotate = {0, 0, 0};
	_worldTransform.scale_ = _scale;
	_currentQuaternion = rotate;
	_beforeRotate = {0, 0, 0};

	_type = type;
	_isDead = false;
}

void Bullet::Update() {
	if (_isDead)
		ToDead();
	Move();

	_worldTransform.translation_ = _pos;
	_worldTransform.rotation_ = _rotate;
	Vector3 frameRotate = _rotate - _beforeRotate;
	Quaternion frameQ = Quaternion::RadianToQuaternion(frameRotate);
	_currentQuaternion = _currentQuaternion * frameQ;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	_beforeRotate = _rotate;

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Bullet::Draw() {
	if (_model != nullptr)
		_model->Draw(_worldTransform, *_viewProjection);
}

void Bullet::Fire() { BulletManager::_updatePool_player.push_back(this); }

const Vector3 Bullet::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

Bullet* BulletManager::AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Bullet::Type type) {
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
