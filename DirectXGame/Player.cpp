#include "Player.h"

void Player::Move() {
	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	// カメラの前方向を計算
	front.x = sinf(_playerRotate.y) * cosf(_playerRotate.x);
	front.y = -sinf(_playerRotate.x);
	front.z = cosf(_playerRotate.y) * cosf(_playerRotate.x);
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
		move = move + right;
		_moveGasPedal *= 0.5f;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {
		move = move + (right * -1);
		_moveGasPedal *= 0.5f;
	}
	_moveGasPedal = std::clamp(_moveGasPedal, -_moveGasMax / 2, _moveGasMax); // 油门速度限制
	move += (front * (_moveGasPedal >= 0 ? 1 : -1));                          // 因为发动机，所以一直都有一个向正前方飞行的力
	// 相对静止控制机制（刹车）
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		_velocity.x = std::lerp(_velocity.x, 0.f, _moveBrakeSpeed);
		_velocity.y = std::lerp(_velocity.y, 0.f, _moveBrakeSpeed);
		_velocity.z = std::lerp(_velocity.z, 0.f, _moveBrakeSpeed);
		_moveGasPedal = std::lerp(_moveGasPedal, 0.f, _moveBrakeSpeed);
	}

	// 鼠标控制镜头旋转
	// Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	// Vector2 mouseRotate = {float(mouseMove.lY), float(mouseMove.lX)};
	//_playerRotate.x += mouseRotate.x * _rotationSpeed * 0.01f;
	//_playerRotate.y += mouseRotate.y * _rotationSpeed * 0.01f;
	_playerRotate.x += (_arrowMouse.y - _arrowMove.y) * _rotationSpeed;
	_playerRotate.y += (_arrowMouse.x - _arrowMove.x) * _rotationSpeed;

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
		_playerPos += _velocity; // 位置移动
	} else {
		_playerPos *= 0.999f;
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
	if (Input::GetInstance()->IsPressMouse(0)) {
		if (_currentTime > _attackTime) {
			_currentTime = 0;
			_bullet = new Bullet();
			_bullet->Initalize(_viewProjection, _playerPos, _playerRotate);
		}
	}
	_currentTime++;
}

Player::~Player() { delete _model; }

void Player::Initalize(ViewProjection* viewProjection, const Vector3& position) {
	_model = Model::CreateFromOBJ("Enemy", true);
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_playerPos = position;
}

void Player::Update() {
	ArrowMove();
	Move();
	Attack();
	if (_bullet != nullptr)
		_bullet->Update();

	_worldTransform.translation_ = _playerPos;
	_worldTransform.rotation_ = _playerRotate;
	_worldTransform.UpdateMatrix(); // 行列計算

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Player::Draw() {
	_model->Draw(_worldTransform, *_viewProjection);
	if (_bullet != nullptr)
		_bullet->Draw();
}

const Vector3 Player::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
