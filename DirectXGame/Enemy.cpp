#include "Enemy.h"

Enemy::~Enemy() { delete _model; }

void Enemy::Initalize(ViewProjection* viewProjection, const Vector3& position, Player* playerObj) {
	_model = Model::CreateFromOBJ("Enemy", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
	_rotate = {0, 0, 0};
	_currentQuaternion = {1, 0, 0, 0};
	_beforeRotate = {0, 0, 0};

	_playerObj = playerObj;

	_currentState = State::Chase;
	Enter_Chase();

	std::srand(static_cast<unsigned int>(time(0)) + int(_pos.x));
}

void Enemy::Update() {
	// 状态机
	switch (_currentState) {
	case Enemy::State::Chase:
		if (IsExit_Chase()) {
			Exit_Chase();
			// 判断条件
			// Enter_Orbit();
			//_currentState = State::Orbit;
		}
		Update_Chase();
		break;
	case Enemy::State::Raid:
		break;
	case Enemy::State::Orbit:
		break;
	case Enemy::State::Flee:
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
}

void Enemy::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

void Enemy::Fire() { EnemyManager::_updatePool.push_back(this); }

bool Enemy::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] <= 0) {
		_currentTimes[index] = frame;
		return true;
	}
	_currentTimes[index]--;
	return false;
}

Quaternion Enemy::RandomZRotation(float maxAngleRadians) {
	// 生成一个在 [-maxAngleRadians, maxAngleRadians] 范围内的随机角度
	float randomAngle = ((float)std::rand() / RAND_MAX) * 2.0f * maxAngleRadians - maxAngleRadians;
	// 返回对应的 Z 轴旋转四元数
	return Quaternion{std::cos(randomAngle / 2), 0.0f, 0.0f, std::sin(randomAngle / 2)};
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

void Enemy::Enter_Chase() {}

void Enemy::Exit_Chase() {}

void Enemy::Update_Chase() {
	// 更新位置
	Quaternion targetQ = Quaternion::LookAt(_pos, _playerObj->GetPos());
	if (FrameTimeWatch(300, 1)) {
		float maxZangle = 30 * std::acosf(-1) / 180; // 随机给Z轴转一下，更自然
		_randomZ = RandomZRotation(maxZangle);
	}
	Quaternion finalQ = _randomZ * targetQ;
	_currentQuaternion = Quaternion::Slerp(_currentQuaternion, finalQ, _rotateSpeed);
	// Quaternion q = _currentQuaternion;
	// Vector3 forward = {2.0f * (q.x * q.z + q.w * q.y), 2.0f * (q.y * q.z - q.w * q.x), 1.0f - 2.0f * (q.x * q.x + q.y * q.y)};
	Vector3 forward = My3dTools::GetDirection_front(_currentQuaternion);
	_accelerations = forward * _speed;

	float length = (_playerObj->GetPos() - _pos).Length();
	if (IsAimingAtPlayer(_aimingRadian) && length < _aimingLength) {
		Attack();
	}
}

bool Enemy::IsExit_Chase() { return false; }

const Vector3 Enemy::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

void EnemyManager::Updata() {
	for (Enemy* it : _updatePool) {
		it->Update();
	}
}

void EnemyManager::Draw() {
	for (Enemy* it : _updatePool) {
		it->Draw();
	}
}

Enemy* EnemyManager::AcquireEnemy(ViewProjection* viewProjection, const Vector3& position, Player* playerObj) {
	if (_idlePool.empty()) {
		Enemy* enemy = new Enemy();
		enemy->Initalize(viewProjection, position, playerObj);
		return enemy;
	} else {
		Enemy* enemy = _idlePool.front();
		_idlePool.pop();
		enemy->Initalize(viewProjection, position, playerObj);
		return enemy;
	}
}

void EnemyManager::ReleaseEnemy(Enemy* enemy) {
	auto it = std::find(_updatePool.begin(), _updatePool.end(), enemy);
	if (it != _updatePool.end()) {
		_updatePool.erase(it);
	}
	_idlePool.push(enemy);
}
