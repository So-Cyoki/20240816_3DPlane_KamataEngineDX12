#include "Player.h"

void Player::Move() {
	float moveSpeed = 0.1f;     // キーボードで移動のスピード
	float rotationSpeed = 0.4f; // マウスの右キーで回るスピード

	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	bool isMouseMove = false;               // マウスで移動
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

	// カメラをマウスで回転
	Vector2 mousePos = Input::GetInstance()->GetMousePosition();
	float mousePosX = mousePos.x, mousePosY = mousePos.y;
	Vector2 currentMousePos{};
	if (Input::GetInstance()->IsPressMouse(1)) {
		// マウスの右キー
		currentMousePos = {float(mousePosX), float(mousePosY)};
		_playerRotate.x += (currentMousePos.y - preMousePos.y) * rotationSpeed * 0.01f;
		_playerRotate.y += (currentMousePos.x - preMousePos.x) * rotationSpeed * 0.01f;
		preMousePos = {float(mousePosX), float(mousePosY)};
	} else {
		preMousePos = {float(mousePosX), float(mousePosY)};
	}

	// 正規化、速度は同じにするために
	if (!isMouseMove) {
		if (move.x != 0 || move.y != 0 || move.z != 0) {
			move.Normalize();
			move = move * moveSpeed;
		}
	}
	_playerPos.x += move.x;
	_playerPos.y += move.y;
	_playerPos.z += move.z;
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
