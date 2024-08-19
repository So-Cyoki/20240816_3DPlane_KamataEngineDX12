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
		move = move + front;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		move = move + (front * -1);
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		move = move + (right * -1);
	} else if (Input::GetInstance()->PushKey(DIK_A)) {
		move = move + right;
	}
	// 相对静止机制
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		_velocity.x = std::lerp(_velocity.x, 0.f, _moveSpeed);
		_velocity.y = std::lerp(_velocity.y, 0.f, _moveSpeed);
		_velocity.z = std::lerp(_velocity.z, 0.f, _moveSpeed);
	}

	// カメラをマウスで回転
	Vector2 mousePos = Input::GetInstance()->GetMousePosition();
	float mousePosX = mousePos.x, mousePosY = mousePos.y;
	Vector2 currentMousePos{};
	if (Input::GetInstance()->IsPressMouse(1)) {
		// マウスの右キー
		currentMousePos = {float(mousePosX), float(mousePosY)};
		_playerRotate.x += (currentMousePos.y - preMousePos.y) * _rotationSpeed * 0.01f;
		_playerRotate.y += (currentMousePos.x - preMousePos.x) * _rotationSpeed * 0.01f;
		preMousePos = {float(mousePosX), float(mousePosY)};
	} else {
		preMousePos = {float(mousePosX), float(mousePosY)};
	}

	// 正規化、速度は同じにするために
	if (move.x != 0 || move.y != 0 || move.z != 0)
		move.Normalize();

	// 物理计算部分
	_accelerations = move * _moveSpeed;
	_velocity += _accelerations;

	_velocity.x = std ::clamp(_velocity.x, -_moveMax, _moveMax);
	_velocity.y = std ::clamp(_velocity.y, -_moveMax, _moveMax);
	_velocity.z = std ::clamp(_velocity.z, -_moveMax, _moveMax);

	_playerPos += _velocity;
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
	Move();

	_worldTransform.translation_ = _playerPos;
	_worldTransform.rotation_ = _playerRotate;
	_worldTransform.UpdateMatrix(); // 行列計算

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

const Vector3 Player::GetWorldPosition() {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
