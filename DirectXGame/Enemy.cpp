#include "Enemy.h"

Enemy::~Enemy() { delete _model; }

void Enemy::Initalize(ViewProjection* viewProjection, const Vector3& position, Player* playerObj, GameUI* gameUIObj) {
	_model = Model::CreateFromOBJ("Enemy", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
	_rotate = {0, 0, 0};
	_currentQuaternion = {1, 0, 0, 0};
	_beforeRotate = {0, 0, 0};

	_playerObj = playerObj;
	_gameUIObj = gameUIObj;
	_isHurt = false;
	_isDead = false;

	_hp = 20;

	_velocity = {0, 0, 0};
	_accelerations = {0, 0, 0};
	_currentState = State::Chase;

	std::srand(static_cast<unsigned int>(time(0)) + int(_pos.x)); // 设定随机种子

	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Enemy::Update() {
	// 生命值检测
	if (_hp <= 0) {
		_isDead = true;
	}
	// 状态机
	switch (_currentState) {
	case Enemy::State::Chase:
		if (IsExit_Chase()) {
			_currentState = State::Flee;
		}
		Update_Chase();
		break;
	case Enemy::State::Raid:
		break;
	case Enemy::State::Orbit:
		break;
	case Enemy::State::Flee:
		if (IsExit_Flee()) {
			_currentState = State::Chase;
		}
		Update_Flee();
		break;
	case Enemy::State::Dead:
		break;
	}

	// 物理移动
	_velocity += _accelerations;
	_velocity.x = std ::clamp(_velocity.x, -_moveMax, _moveMax);
	_velocity.y = std ::clamp(_velocity.y, -_moveMax, _moveMax);
	_velocity.z = std ::clamp(_velocity.z, -_moveMax, _moveMax);
	_pos += _velocity;

	// 碰撞
	IsCollision();

	// 旋转必须使用四元数，而且是每帧计算的四元数，这才可以保证旋转绝对是没问题的
	// 现在直接控制四元数来决定它的旋转方向，所以不会去调整rotate的值
	// 而且在状态机中，会直接修改_currentQuaternion的值，来让敌人指向一个位置，所以不要在这里调整
	//_worldTransform.rotation_ = _rotate;
	// Vector3 frameRotate = _rotate - _beforeRotate;
	// Quaternion frameQ = Quaternion::RadianToQuaternion(frameRotate);
	//_currentQuaternion = _currentQuaternion * frameQ;
	_currentQuaternion.normalize();
	_worldTransform.translation_ = _pos;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	//_beforeRotate = _rotate;

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
	_isHurt = false;

	Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
	// 粒子效果
	if (FrameTimeWatch(_pMoveTime, 2, true)) {
		// ParticleManager::ADD_Move(_viewProjection, _pos + front * -7, _currentQuaternion);
	}
}

void Enemy::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Enemy::Fire() { EnemyManager::_updatePool.push_back(this); }

void Enemy::ToDead() {
	// 物理移动
	_pos += _velocity;
	_currentQuaternion.normalize();
	Quaternion rotate = {1, 0.01f, 0.01f, 0.01f};
	_currentQuaternion = rotate * _currentQuaternion;
	_worldTransform.translation_ = _pos;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
	// UI
	_gameUIObj->AniStart_help();
	// Particle
	if (FrameTimeWatch(_deadTime, 2, false)) {
		_gameUIObj->DeadScore();
		ParticleManager::ADD_Boom3(_viewProjection, _pos, _currentQuaternion);
		EnemyManager::ReleaseEnemy(this);
	} else {
		if (FrameTimeWatch(_deadTime_boom, 3, true)) {
			ParticleManager::ADD_Boom(_viewProjection, _pos, _currentQuaternion);
		}
		if (FrameTimeWatch(_deadTime_boom2, 4, true)) {
			ParticleManager::ADD_Boom2(_viewProjection, _pos, _currentQuaternion);
		}
	}
}

void Enemy::Update_Flee() {
	// 计算前进方向
	Vector3 forward = My3dTools::GetDirection_front(_currentQuaternion);
	_accelerations = forward * _speed;
	// 攻击
	float length = (_playerObj->GetPos() - _pos).Length();
	if (IsAimingAtPlayer(_aimingRadian) && length < _aimingLength) {
		Attack();
	}
}

bool Enemy::IsExit_Flee() {
	if (FrameTimeWatch(_fleeTime, 1, false))
		return true;
	return false;
}

bool Enemy::FrameTimeWatch(int frame, int index, bool first) {
	if (!first) {
		if (_currentTimes[index] > frame) {
			_currentTimes[index] = 0;
			return true;
		}
		_currentTimes[index]++;
	} else {
		if (_currentTimes[index] <= 0) {
			_currentTimes[index] = frame;
			return true;
		}
		_currentTimes[index]--;
	}
	return false;
}

Quaternion Enemy::RandomZRotation(float maxAngleRadians) {
	// 生成一个在 [-maxAngleRadians, maxAngleRadians] 范围内的随机角度
	float randomAngle = ((float)std::rand() / RAND_MAX) * 2.0f * maxAngleRadians - maxAngleRadians;
	// 返回对应的 Z 轴旋转四元数
	return Quaternion{std::cosf(randomAngle / 2), 0.0f, 0.0f, std::sinf(randomAngle / 2)};
}

void Enemy::Attack() {
	Vector3 offset = {1, 0, 3}; // 调整子弹的出现位置

	Vector3 up = My3dTools::GetDirection_up(_currentQuaternion);
	Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
	Vector3 right = My3dTools::GetDirection_right(_currentQuaternion);
	Vector3 bulletBornPos1 = _pos + up * offset.y + front * offset.z + right * offset.x;
	Vector3 bulletBornPos2 = _pos + up * offset.y + front * offset.z + right * -offset.x;
	Vector3 bulletBornRotate = {_rotate.x, _rotate.y, 0};
	Bullet* bullet1 = BulletManager::AcquireBullet(_viewProjection, bulletBornPos1, _currentQuaternion, Bullet::tEnemy);
	bullet1->Fire();
	Bullet* bullet2 = BulletManager::AcquireBullet(_viewProjection, bulletBornPos2, _currentQuaternion, Bullet::tEnemy);
	bullet2->Fire();
}

void Enemy::IsCollision() {
	// Bullet
	for (Bullet* bullet : BulletManager::_updatePool_player) {
		if (My3dTools::IsCollision(bullet->GetSphere(), _sphere)) {
			bullet->SetIsHurt(true);

			_isHurt = true;
			_hp -= _playerObj->GetAttackValue();
			// 给一个冲击的力
			Vector3 front = My3dTools::GetDirection_front(bullet->GetQuaternion());
			_velocity += front * _hurtVel;
			// UI
			_gameUIObj->HurtScore();
			_gameUIObj->AniStart_skillPoint();
		}
	}
	// Player
	if (My3dTools::IsCollision(_sphere, _playerObj->GetSphere())) {
		Vector3 vel = _playerObj->GetVelocity();
		vel *= -2;
		_playerObj->SetVelocity(vel);
		_velocity *= -10;

		Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
		ParticleManager::ADD_Hurt(_viewProjection, _pos + (front * 8), _currentQuaternion);
		ParticleManager::ADD_Hurt(_viewProjection, _pos + (front * 8), _currentQuaternion);
		ParticleManager::ADD_Hurt(_viewProjection, _pos + (front * 8), _currentQuaternion);

		_hp -= _playerObj->GetAttackValue() * 10;
		float playerHp = _playerObj->GetHp();
		playerHp -= _playerObj->GetAttackValue() * 5;
		_playerObj->SetHp(playerHp);
	}
}

bool Enemy::IsAimingAtPlayer(float maxAimAngleRadians) {
	// 1. 计算敌人的前向向量
	Vector3 forward = My3dTools::GetDirection_front(_currentQuaternion);

	// 2. 计算指向玩家的方向向量
	Vector3 directionToPlayer = _playerObj->GetPos() - _pos;
	directionToPlayer = directionToPlayer.Normalize();

	// 3. 计算前向向量与玩家方向向量的夹角
	float dot = Vector3::Dot(forward, directionToPlayer);
	float angleBetween = std::acosf(dot);

	// 4. 判断是否在瞄准范围内
	return angleBetween <= maxAimAngleRadians;
}

void Enemy::Update_Chase() {
	// 跟踪目标
	Quaternion targetQ = Quaternion::LookAt(_pos, _playerObj->GetPos());
	if (FrameTimeWatch(300, 0, false)) {
		float maxZangle = 30 * std::acosf(-1) / 180; // 随机给Z轴转一下，更自然
		_randomZ = RandomZRotation(maxZangle);
	}
	Quaternion finalQ = _randomZ * targetQ;
	_currentQuaternion = Quaternion::Slerp(_currentQuaternion, finalQ, _rotateSpeed);
	// 计算前进方向
	Vector3 forward = My3dTools::GetDirection_front(_currentQuaternion);
	_accelerations = forward * _speed;
	// 攻击
	float length = (_playerObj->GetPos() - _pos).Length();
	if (IsAimingAtPlayer(_aimingRadian) && length < _aimingLength) {
		Attack();
	}
}

bool Enemy::IsExit_Chase() {
	if (_isHurt)
		return true;
	return false;
}

const Vector3 Enemy::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

void EnemyManager::Updata() {
	for (Enemy* it : _updatePool) {
		if (!it->GetIsDead())
			it->Update();
		else
			it->ToDead();
	}
}

void EnemyManager::Draw() {
	for (Enemy* it : _updatePool) {
		it->Draw();
	}
}

void EnemyManager::EnemyBornSystem(ViewProjection* viewProjection, Player* playerObj, GameUI* gameUIObj) {
	if (!_isStart) {
		Enemy* enemy = nullptr;
		Vector3 tempV = {40, 0, 0};
		enemy = AcquireEnemy(viewProjection, _startPos + tempV, playerObj, gameUIObj);
		enemy->Fire();
		enemy = AcquireEnemy(viewProjection, _startPos + (tempV * -1), playerObj, gameUIObj);
		enemy->Fire();
		_isStart = true;
	}

	int size = static_cast<int>(_updatePool.size());
	if (size < _enemyMin) {
		Vector3 bornPos = My3dTools::GetDirection_front(playerObj->GetQuaternion()) * -300;
		Enemy* enemy = AcquireEnemy(viewProjection, bornPos, playerObj, gameUIObj);
		enemy->Fire();
	}
	if (size < _enemyMax && FrameTimeWatch(_bornTime, 0, false)) {
		Vector3 bornPos = My3dTools::GetDirection_front(playerObj->GetQuaternion()) * -300;
		Enemy* enemy = AcquireEnemy(viewProjection, bornPos, playerObj, gameUIObj);
		enemy->Fire();
	}
	if (size <= 0) {
		Enemy* enemy = nullptr;
		Vector3 tempV = {40, 0, 0};
		Vector3 bornPos = My3dTools::GetDirection_front(playerObj->GetQuaternion()) * -300;
		enemy = AcquireEnemy(viewProjection, bornPos, playerObj, gameUIObj);
		enemy->Fire();
		enemy = AcquireEnemy(viewProjection, bornPos + tempV, playerObj, gameUIObj);
		enemy->Fire();
		enemy = AcquireEnemy(viewProjection, bornPos + (tempV * -1), playerObj, gameUIObj);
		enemy->Fire();
	}
}

bool EnemyManager::FrameTimeWatch(int frame, int index, bool first) {
	if (!first) {
		if (_currentTimes[index] > frame) {
			_currentTimes[index] = 0;
			return true;
		}
		_currentTimes[index]++;
	} else {
		if (_currentTimes[index] <= 0) {
			_currentTimes[index] = frame;
			return true;
		}
		_currentTimes[index]--;
	}
	return false;
}

Enemy* EnemyManager::AcquireEnemy(ViewProjection* viewProjection, const Vector3& position, Player* playerObj, GameUI* gameUIObj) {
	if (_idlePool.empty()) {
		Enemy* enemy = new Enemy();
		enemy->Initalize(viewProjection, position, playerObj, gameUIObj);
		return enemy;
	} else {
		Enemy* enemy = _idlePool.front();
		_idlePool.pop();
		enemy->Initalize(viewProjection, position, playerObj, gameUIObj);
		return enemy;
	}
}

void EnemyManager::ReleaseEnemy(Enemy* enemy) {
	auto it = std::find(_updatePool.begin(), _updatePool.end(), enemy);
	if (it != _updatePool.end()) {
		std::swap(*it, _updatePool.back());
		_updatePool.pop_back();
		_idlePool.push(enemy);
	}
}
