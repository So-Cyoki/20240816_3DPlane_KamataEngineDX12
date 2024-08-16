#include "CameraController.h"

void CameraController::Update() {
	_viewProjection.translation_ = _cameraPos;
	_viewProjection.rotation_ = _cameraRotate;
	_viewProjection.UpdateMatrix();
}
