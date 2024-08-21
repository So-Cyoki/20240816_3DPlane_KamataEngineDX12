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

Bullet::~Bullet() { delete _model; }

void Bullet::Initalize(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate) {
	_model = Model::CreateFromOBJ("Bullet", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_worldTransform.rotation_ = rotate;
	_pos = position;
	_rotate = rotate;
	_worldTransform.scale_ = _scale;
}

void Bullet::Update() {
	Move();

	_worldTransform.translation_ = _pos;
	_worldTransform.rotation_ = _rotate;
	_worldTransform.UpdateMatrix(); // 行列計算

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Bullet::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Bullet::Fire() { BulletManager::_updatePool.push_back(this); }

const Vector3 Bullet::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

Bullet* BulletManager::AcquireBullet(ViewProjection* viewProjection, const Vector3& position, const Vector3& rotate) {
	if (_idlePool.empty()) {
		Bullet* bullet = new Bullet();
		bullet->Initalize(viewProjection, position, rotate);
		return bullet;
	} else {
		Bullet* bullet = _idlePool.front();
		_idlePool.pop();
		bullet->Initalize(viewProjection, position, rotate);
		return bullet;
	}
}

void BulletManager::ReleaseBullet(Bullet* bullet) {
	auto it = std::find(_updatePool.begin(), _updatePool.end(), bullet);
	if (it != _updatePool.end()) {
		_updatePool.erase(it);
	}
	_idlePool.push(bullet);
}

void BulletManager::Updata() {
	for (Bullet* it : _updatePool) {
		it->Update();
	}
}

void BulletManager::Draw() {
	for (Bullet* it : _updatePool) {
		it->Draw();
	}
}
