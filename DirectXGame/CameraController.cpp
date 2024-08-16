#include "CameraController.h"

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = _target->GetWorldTransform();
	_targetPos = targetWorldTransform.translation_ + _targetOffset;
	_cameraPos = _targetPos;

	_viewProjection.translation_ = _cameraPos;
	_viewProjection.rotation_ = _cameraRotate;
	_viewProjection.UpdateMatrix();
}
