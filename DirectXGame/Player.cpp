#include "Player.h"

void Player::Move() {
	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	// カメラの前方向を計算
	front.x = sinf(_rotate.y) * cosf(_rotate.x);
	front.y = -sinf(_rotate.x);
	front.z = cosf(_rotate.y) * cosf(_rotate.x);
	front.Normalize();
	// カメラの横方向を計算
	Vector3 worldUp{0, 1, 0};
	right = Vector3::Cross(front, worldUp);
	right.Normalize();
	// カメラの上方向を計算
	up = Vector3::Cross(right, front);
	up.Normalize();

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
	// Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	// Vector2 mouseRotate = {float(mouseMove.lY), float(mouseMove.lX)};
	//_rotate.x += mouseRotate.x * _rotationSpeed * 0.01f;
	//_rotate.y += mouseRotate.y * _rotationSpeed * 0.01f;
	// 因为x在90 ~270度的时候，上下被翻转了，所以这时候左转就会让镜头右转，因此需要特别判断一下，更符合直观
	float absRotateX = std::fabsf(_rotate.x);
	if (absRotateX > acosf(-1) * 0.5f && absRotateX < acosf(-1) * 1.5f)
		_rotate.y -= (_arrowMouse.x - _arrowMove.x) * _rotationSpeed;
	else
		_rotate.y += (_arrowMouse.x - _arrowMove.x) * _rotationSpeed;
	_rotate.x += (_arrowMouse.y - _arrowMove.y) * _rotationSpeed;
	// 随着左右偏移飞机也做出左右翻转的动画
	// if ((_arrowMouse.x - _arrowMove.x) > 0)
	//	_rotate.z = std::lerp(_rotate.z, -1.f, 0.01f);
	// else
	//	_rotate.z = std::lerp(_rotate.z, 1.f, 0.01f);

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

	// 最大范围限制，避免飞出天空球
	Vector3 preWorldPos = GetWorldPosition();
	if (_moveMaxLength > preWorldPos.Length()) {
		_pos += _velocity; // 位置移动
	} else {
		_pos *= 0.999f;
	}
}

void Player::ArrowMove() {
	// 不断的返回屏幕中心点
	_arrowMouse.x = std::lerp(_arrowMouse.x, _screenWidth / 2, _rotationSpeed * 1000);
	_arrowMouse.y = std::lerp(_arrowMouse.y, _screenHeight / 2, _rotationSpeed * 1000);
	// 根据鼠标移动
	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	Vector2 mouseMovePos = {float(mouseMove.lX), float(mouseMove.lY)};
	_arrowMouse += mouseMovePos;
}

void Player::Attack() {
	if (Input::GetInstance()->IsPressMouse(0) && FrameTimeWatch(_attackTime, 1)) {
		Vector3 up = My3dTools::GetDirection_up(_rotate);
		Vector3 front = My3dTools::GetDirection_front(_rotate);
		Vector3 bulletBornPos = _pos + up * 1 + front * -10;
		Bullet* bullet = BulletManager::AcquireBullet(_viewProjection, bulletBornPos, _rotate, Bullet::tPlayer);
		bullet->Fire();
	}
}

bool Player::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] <= 0) {
		_currentTimes[index] = frame;
		return true;
	}
	_currentTimes[index]--;
	return false;
}

Player::~Player() { delete _model; }

void Player::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_model = Model::CreateFromOBJ("Enemy", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
}

void Player::Update() {
	ArrowMove();
	Move();
	Attack();

	_worldTransform.translation_ = _pos;
	_worldTransform.rotation_ = _rotate;
	_worldTransform.UpdateMatrix(); // 行列計算

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

const Vector3 Player::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
