#include "CameraController.h"

void CameraController::Initialize() {
	_viewProjection.Initialize();
	_currentQuaternion = {1, 0, 0, 0};
	_beforeRotate = {0, 0, 0};

	_pos = {0, 20, -100};
	_rotate = {0.4f, 0, 0};
}

void CameraController::Update() {
	if (_target != nullptr) {
		// 获取目标的位置和旋转信息
		const WorldTransform& targetWorldTransform = _target->GetWorldTransform();

		// Vector3 targetFront = My3dTools::GetDirection_front(_target->GetQuaternion());
		// Vector3 targetUp = My3dTools::GetDirection_up(_target->GetQuaternion());
		// Vector3 targetRight = My3dTools::GetDirection_right(_target->GetQuaternion());
		Vector3 targetFront = My3dTools::GetDirection_front(_currentQuaternion);
		Vector3 targetUp = My3dTools::GetDirection_up(_currentQuaternion);
		Vector3 targetRight = My3dTools::GetDirection_right(_currentQuaternion);
		// 跟随的目标的位置，并且加上自定义的位置偏移
		_targetPos = targetWorldTransform.translation_ + targetFront * _targetOffset.z + targetUp * _targetOffset.y + targetRight * _targetOffset.x;
		_targetRotate = targetWorldTransform.rotation_;

		_pos = _targetPos;
		_rotate = _targetRotate;
		//_currentQuaternion = _target->GetQuaternion();
	}

	// 重新传入摄像机的view矩阵中
	_viewProjection.translation_ = _pos;
	_viewProjection.rotation_ = _rotate;
	Vector3 frameRotate = _rotate - _beforeRotate;
	Quaternion frameQ = Quaternion::RadianToQuaternion(frameRotate);
	_currentQuaternion = _currentQuaternion * frameQ;
	Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _currentQuaternion, _pos);
	// Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _rotate, _pos);
	_viewProjection.matView = m.Inverse();
	_viewProjection.TransferMatrix();
	_beforeRotate = _rotate;
}
