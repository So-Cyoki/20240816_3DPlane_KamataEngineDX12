#include "Bullet.h"

void Bullet::Move() {
	Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
	_pos += front * _speed;
}

void Bullet::ToDead() {
	if (_isHurt)
		ParticleManager::ADD_Hurt(_viewProjection, _pos, _currentQuaternion);
	BulletManager::ReleaseBullet(this);
}

bool Bullet::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] > frame) {
		_currentTimes[index] = 0;
		return true;
	}
	_currentTimes[index]++;
	return false;
}

Bullet::~Bullet() { delete _model; }

void Bullet::Initalize(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Type type) {
	//_model = Model::CreateFromOBJ("Bullet", true);
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
	_isHurt = false;
	// 必须在初始化的时候就更新world矩阵和碰撞体呀！不然肯定会重复判定的呀！
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Bullet::Update() {
	// 存活时间限制
	if (FrameTimeWatch(_lifeTime, 0))
		_isDead = true;
	// 死亡写在了Manager里面，这样才是最准确的
	Move();

	// 因为直接获取旋转四元数，所以为了优化就不需要额外计算了
	//_worldTransform.rotation_ = _rotate;
	//  Vector3 frameRotate = _rotate - _beforeRotate;
	//  Quaternion frameQ = Quaternion::RadianToQuaternion(frameRotate);
	//_currentQuaternion = _currentQuaternion * frameQ;
	_worldTransform.translation_ = _pos;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	//_beforeRotate = _rotate;

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Bullet::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Bullet::Fire() {
	switch (_type) {
	case Bullet::tPlayer:
		BulletManager::_updatePool_player.push_back(this);
		break;
	case Bullet::tEnemy:
		BulletManager::_updatePool_enemy.push_back(this);
		break;
	}
}

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
			std::swap(*it, _updatePool_player.back());
			_updatePool_player.pop_back();
			_idlePool.push(bullet); // 我真是有毒，肯定是要找到这个子弹才放进Idle池啊！
		}
		break;
	}
	case Bullet::tEnemy: {
		auto it = std::find(_updatePool_enemy.begin(), _updatePool_enemy.end(), bullet);
		if (it != _updatePool_enemy.end()) {
			std::swap(*it, _updatePool_enemy.back());
			_updatePool_enemy.pop_back();
			_idlePool.push(bullet);
		}
		break;
	}
	}
}

void BulletManager::Updata() {
	for (Bullet* it : _updatePool_player) {
		if (!it->GetIsDead())
			it->Update();
		else
			it->ToDead();
	}
	for (Bullet* it : _updatePool_enemy) {
		if (!it->GetIsDead())
			it->Update();
		else
			it->ToDead();
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
