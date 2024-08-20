#include "EarthBall.h"

void EarthBall::IsCollision() {
	bool IsPlayer = My3dTools::IsCollision(_sphere, _playerObj->GetSphere());
	if (IsPlayer) {
		Vector3 vel = _playerObj->GetVelocity();
		vel *= -1;
		_playerObj->SetVelocity(vel);
	}
}

EarthBall::~EarthBall() { delete _model; }

void EarthBall::Initialize(ViewProjection* view, Player* playerObj) {
	_model = Model::CreateFromOBJ("EarthBall", true);
	_worldTransform.Initialize();
	_viewProjection = view;
	_playerObj = playerObj;

	_worldTransform.scale_ = {_radius, _radius, _radius};
	_worldTransform.rotation_ = {0, 0, -23.5f * acosf(-1) / 180.f};
}

void EarthBall::Update() {
	// 地区的自转
	_worldTransform.rotation_ += _rotationValue;

	_worldTransform.UpdateMatrix();

	_sphere = My3dTools::GetSphere(_radius, GetWorldPosition());
	IsCollision();
}

void EarthBall::Draw() { _model->Draw(_worldTransform, *_viewProjection); }

const Vector3 EarthBall::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}
