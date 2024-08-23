#include "CameraController.h"

void CameraController::Update() {
	// 获取目标的位置和旋转信息
	const WorldTransform& targetWorldTransform = _target->GetWorldTransform();

	Vector3 targetFront = My3dTools::GetDirection_front(targetWorldTransform.rotation_);
	Vector3 targetUp = My3dTools::GetDirection_up(targetWorldTransform.rotation_);
	Vector3 targetRight = My3dTools::GetDirection_right(targetWorldTransform.rotation_);
	// 跟随的目标的位置，并且加上自定义的位置偏移
	_targetPos = targetWorldTransform.translation_ + targetFront * _targetOffset.z + targetUp * _targetOffset.y + targetRight * _targetOffset.x;
	// 不需要目标的旋转Z轴，不然镜头也会一起翻转，容易吐
	_targetRotate = {targetWorldTransform.rotation_.x, targetWorldTransform.rotation_.y, _targetRotate.z};

	_cameraPos = _targetPos;
	_cameraRotate = _targetRotate;
	// 重新传入摄像机的view矩阵中
	Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _cameraRotate, _cameraPos);
	_viewProjection.matView = m.Inverse();
	_viewProjection.TransferMatrix();
}
