#include "Player.h"

void Player::Move() {
	// 移動
	if (Input::GetInstance()->PushKey(DIK_A)) {
		_playerPos.x -= _speed;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {
		_playerPos.x += _speed;
	}
	if (Input::GetInstance()->PushKey(DIK_W)) {
		_playerPos.z += _speed;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		_playerPos.z -= _speed;
	}
}

void Player::MouseCamera(Vector3* cameraPos, Vector3* cameraRotate) {
	cameraPos, cameraRotate;
	float moveSpeed = 0.1f;     // キーボードで移動のスピード
	float wheelSpeed = 0.3f;    // マウスのホイールスクロールのスピード
	float rotationSpeed = 0.4f; // マウスの右キーで回るスピード
	float dragSpeed = 0.5f;     // マウスの中キーで移動のスピード

	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	bool isMouseMove = false;               // マウスで移動
	                                        //  カメラの前方向を計算
	front.x = sinf(cameraRotate->y) * cosf(cameraRotate->x);
	front.y = -sinf(cameraRotate->x);
	front.z = cosf(cameraRotate->y) * cosf(cameraRotate->x);
	front.Normalize();
	//// カメラの横方向を計算
	// Vector3 worldUp{0, 1, 0};
	// right = Cross(front, worldUp);
	// right = Normalize(right);
	//// カメラの上方向を計算
	// up = Cross(right, front);
	// up = Normalize(up);

	// if (Novice::IsPressMouse(1)) {
	//	// キーボードで移動
	//	if (key[DIK_W]) {
	//		move = Add(move, front);
	//	} else if (key[DIK_S]) {
	//		move = Add(move, Multiply(-1, front));
	//	}
	//	if (key[DIK_D]) {
	//		move = Add(move, Multiply(-1, right));
	//	} else if (key[DIK_A]) {
	//		move = Add(move, right);
	//	}
	// } else {
	//	// マウスのホイールスクロール
	//	isMouseMove = true;
	//	move = Multiply(float(Novice::GetWheel()) * wheelSpeed * 0.01f, front);
	// }

	//// カメラをマウスで回転
	// int mousePosX, mousePosY;
	// Novice::GetMousePosition(&mousePosX, &mousePosY);
	// Vector2 currentMousePos{};
	// if (Novice::IsPressMouse(1)) {
	//	// マウスの右キー
	//	currentMousePos = {float(mousePosX), float(mousePosY)};
	//	cameraRotate->x += (currentMousePos.y - preMousePos.y) * rotationSpeed * 0.01f;
	//	cameraRotate->y += (currentMousePos.x - preMousePos.x) * rotationSpeed * 0.01f;
	//	preMousePos = {float(mousePosX), float(mousePosY)};
	// } else if (Novice::IsPressMouse(2)) {
	//	// マウスの中キー
	//	isMouseMove = true;
	//	currentMousePos = {float(mousePosX), float(mousePosY)};
	//	Vector3 mouseVector = {currentMousePos.x - preMousePos.x, currentMousePos.y - preMousePos.y, 0};
	//	if (abs(mouseVector.x) > 1 || abs(mouseVector.y) > 1) {
	//		move = Add(move, Multiply(mouseVector.x * dragSpeed * 0.01f, right));
	//		move = Add(move, Multiply(mouseVector.y * dragSpeed * 0.01f, up));
	//	}
	//	preMousePos = {float(mousePosX), float(mousePosY)};
	// } else {
	//	preMousePos = {float(mousePosX), float(mousePosY)};
	// }

	//// 正規化、速度は同じにするために
	// if (!isMouseMove) {
	//	if (move.x != 0 || move.y != 0 || move.z != 0) {
	//		move = Normalize(move);
	//		move = Multiply(moveSpeed, move);
	//	}
	// }
	// cameraPos->x += move.x;
	// cameraPos->y += move.y;
	// cameraPos->z += move.z;
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
