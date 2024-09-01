#include "Player.h"

void Player::Move() {
	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	front = My3dTools::GetDirection_front(_currentQuaternion);
	right = My3dTools::GetDirection_right(_currentQuaternion);
	up = My3dTools::GetDirection_up(_currentQuaternion);

	// キーボードで移動
	if (Input::GetInstance()->PushKey(DIK_W)) {
		// move = move + front;
		_moveGasPedal += _moveSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		// move = move + (front * -1);
		_moveGasPedal -= _moveSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		_rotate.z += _adRotationSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {
		_rotate.z -= _adRotationSpeed;
	}
	_moveGasPedal = std::clamp(_moveGasPedal, -_moveGasMax / 2, _moveGasMax); // 油门速度限制
	move += (front * (_moveGasPedal >= 0 ? 1 : -1));                          // 因为发动机，所以一直都有一个向正前方飞行的力
	// 相对静止控制机制（刹车，和Z轴回正）
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		_velocity.x = std::lerp(_velocity.x, 0.f, _moveBrakeSpeed);
		_velocity.y = std::lerp(_velocity.y, 0.f, _moveBrakeSpeed);
		_velocity.z = std::lerp(_velocity.z, 0.f, _moveBrakeSpeed);
		_rotate.z = std::lerp(_rotate.z, 0.f, _adRotationSpeed);
		_moveGasPedal = std::lerp(_moveGasPedal, 0.f, _moveBrakeSpeed);
	}

	// 鼠标控制镜头旋转
	// 因为x在90 ~270度的时候，上下被翻转了，所以这时候左转就会让镜头右转，因此需要特别判断一下，更符合直观(使用了四元数后不会出现这个问题了，但是先留着)
	float absRotateX = std::fabsf(_rotate.x);
	if (absRotateX > acosf(-1) * 0.5f && absRotateX < acosf(-1) * 1.5f)
		_rotate.y += (_arrowMouse.x - _arrowMove.x) * _rotationSpeed;
	else
		_rotate.y += (_arrowMouse.x - _arrowMove.x) * _rotationSpeed;
	_rotate.x += (_arrowMouse.y - _arrowMove.y) * _rotationSpeed;

	// 将角度限制在360度之中，方便计算
	_rotate.x = std::fmodf(_rotate.x, acosf(-1) * 2);
	_rotate.y = std::fmodf(_rotate.y, acosf(-1) * 2);
	_rotate.z = std::fmodf(_rotate.z, acosf(-1) * 2);

	// 正規化、速度は同じにするために
	if (move.x != 0 || move.y != 0 || move.z != 0)
		move.Normalize();

	// 物理计算部分
	_accelerations = move * std::fabsf(_moveGasPedal);
	_velocity += _accelerations;

	// 速度限制
	_velocity.x = std ::clamp(_velocity.x, -_moveMax, _moveMax);
	_velocity.y = std ::clamp(_velocity.y, -_moveMax, _moveMax);
	_velocity.z = std ::clamp(_velocity.z, -_moveMax, _moveMax);

	Vector3 preWorldPos = GetWorldPosition();
	if (_moveMaxLength > preWorldPos.Length()) {
		// 位置移动
		_pos += _velocity;
	} else {
		// 最大范围限制
		float scale = _moveMaxLength / preWorldPos.Length();
		_pos *= scale;
	}

	// 粒子效果
	if (FrameTimeWatch_true(_pMoveTime, 2)) {
		// ParticleManager::ADD_Move(_viewProjection, _pos + front * -7, _currentQuaternion);
	}
}

void Player::ArrowMove() {
	// 不断的返回屏幕中心点
	//_arrowMouse.x = std::lerp(_arrowMouse.x, _screenWidth / 2, _rotationSpeed * 1000);
	//_arrowMouse.y = std::lerp(_arrowMouse.y, _screenHeight / 2, _rotationSpeed * 1000);

	// 根据鼠标移动
	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	Vector2 mouseMovePos = {float(mouseMove.lX), float(mouseMove.lY)};
	_arrowMouse += mouseMovePos;
	// 限制在UI的范围内
	float length = (_arrowMouse - _screenPoint).Length();
	if (length > _arrowMouseMax) {
		float scale = _arrowMouseMax / length;
		_arrowMouse = _screenPoint + ((_arrowMouse - _screenPoint) * scale);
	}
}

void Player::Attack() {
	if (Input::GetInstance()->IsPressMouse(0) && FrameTimeWatch_true(_attackTime, 1)) {
		Vector3 offset = {1, 1, 2}; // 调整子弹的出现位置

		Vector3 up = My3dTools::GetDirection_up(_currentQuaternion);
		Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
		Vector3 right = My3dTools::GetDirection_right(_currentQuaternion);
		Vector3 bulletBornPos1 = _pos + up * offset.y + front * offset.z + right * offset.x;
		Vector3 bulletBornPos2 = _pos + up * offset.y + front * offset.z + right * -offset.x;
		Vector3 bulletBornRotate = {_rotate.x, _rotate.y, 0};
		Bullet* bullet1 = BulletManager::AcquireBullet(_viewProjection, bulletBornPos1, _currentQuaternion, Bullet::tPlayer);
		bullet1->Fire();
		Bullet* bullet2 = BulletManager::AcquireBullet(_viewProjection, bulletBornPos2, _currentQuaternion, Bullet::tPlayer);
		bullet2->Fire();
	}
}

void Player::IsCollision() {
	// Bullet
	for (Bullet* bullet : BulletManager::_updatePool_enemy) {
		if (My3dTools::IsCollision(bullet->GetSphere(), _sphere)) {
			bullet->SetIsHurt(true);

			_hp -= _hurtHp;
			// 给一个冲击的力
			Vector3 front = My3dTools::GetDirection_front(bullet->GetQuaternion());
			_velocity += front * _hurtVel;
		}
	}
}

void Player::ToDead() {
	_hp = 0;
	_pos += _velocity;
	Quaternion rotate = {1, 0.01f, 0.01f, 0.01f};
	_currentQuaternion = rotate * _currentQuaternion;

	_worldTransform.translation_ = _pos;
	_worldTransform.rotation_ = _rotate;
	_currentQuaternion.normalize();
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();

	if (FrameTimeWatch_true(_pDeadTime, 3))
		ParticleManager::ADD_Boom(_viewProjection, _pos, _currentQuaternion);
}

bool Player::FrameTimeWatch_true(int frame, int index) {
	if (_currentTimes[index] <= 0) {
		_currentTimes[index] = frame;
		return true;
	}
	_currentTimes[index]--;
	return false;
}

Player::~Player() { delete _model; }

void Player::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
	_rotate = {0, 0, 0};
	_currentQuaternion = {1, 0, 0, 0};
	_beforeRotate = {0, 0, 0};

	_velocity = {0, 0, 0};
	_accelerations = {0, 0, 0};
	_moveGasPedal = 0;
	_arrowMove = _screenPoint;
	_arrowMouse = _screenPoint;

	_isDead = false;
	_hp = _hpMax;

	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Player::Update() {
	// 生命检测
	if (_hp <= 0) {
		_isDead = true;
	}
	if (_isDead) {
		ToDead();
	} else {
		if (!Input::GetInstance()->PushKey(DIK_LSHIFT))
			ArrowMove();
		Move();
		Attack();
		IsCollision();

		// 旋转必须使用四元数，而且是每帧计算的四元数，这才可以保证旋转绝对是没问题的
		_worldTransform.translation_ = _pos;
		_worldTransform.rotation_ = _rotate;
		Vector3 frameRotate = _rotate - _beforeRotate;
		Quaternion frameQ = Quaternion::RadianToQuaternion(frameRotate);
		_currentQuaternion = _currentQuaternion * frameQ;
		_currentQuaternion.normalize();
		_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
		_worldTransform.TransferMatrix();
		_beforeRotate = _rotate;

		_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
	}
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

const Vector3 Player::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
