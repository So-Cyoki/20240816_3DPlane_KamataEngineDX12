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

bool Enemy::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] <= 0) {
		_currentTimes[index] = frame;
		return true;
	}
	_currentTimes[index]--;
	return false;
}

void Enemy::Enter_Chase() {}

void Enemy::Exit_Chase() {}

void Enemy::Update_Chase() {
	// 更新位置
	Quaternion targetQ = Quaternion::LookAt(_pos, _playerObj->GetPos());
	_currentQuaternion = Quaternion::Slerp(_currentQuaternion, targetQ, _rotateSpeed);
	// Quaternion q = _currentQuaternion;
	// Vector3 forward = {2.0f * (q.x * q.z + q.w * q.y), 2.0f * (q.y * q.z - q.w * q.x), 1.0f - 2.0f * (q.x * q.x + q.y * q.y)};
	Vector3 forward = My3dTools::GetDirection_front(_currentQuaternion);
	_pos += forward * _speed;
}

bool Enemy::IsExit_Chase() { return false; }

const Vector3 Enemy::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}