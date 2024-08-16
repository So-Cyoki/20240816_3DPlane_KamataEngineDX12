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
}

void Player::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
