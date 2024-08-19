#include "CameraController.h"

void CameraController::Update() {
	// 获取目标的位置和旋转信息
	const WorldTransform& targetWorldTransform = _target->GetWorldTransform();
	// 其实这样做是没办法达成我要的要求的，因为旋转后方向就会改变了
	_targetPos = targetWorldTransform.translation_ + _targetOffset; // 可以加上喜欢的偏移
	_targetRotate = targetWorldTransform.rotation_;
	_cameraPos = _targetPos;
	_cameraRotate = _targetRotate;
	// 重新传入摄像机的矩阵中
	_viewProjection.translation_ = _cameraPos;
	_viewProjection.rotation_ = _cameraRotate;
	_viewProjection.UpdateMatrix();
}
