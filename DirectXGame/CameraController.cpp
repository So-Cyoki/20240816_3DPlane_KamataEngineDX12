#include "CameraController.h"

void CameraController::Initialize() {
	_viewProjection.Initialize();
	_currentQuaternion = {1, 0, 0, 0};
	_currentQuaternion = _rotateOffset * _currentQuaternion;

	_isStart = false;
	_isStartAni = false;
}

void CameraController::Update() {
	if (_target != nullptr && !_target->GetIsDead()) {
		// 获取目标的位置和旋转信息
		const WorldTransform& targetWorldTransform = _target->GetWorldTransform();

		Vector3 targetFront = My3dTools::GetDirection_front(_currentQuaternion);
		Vector3 targetUp = My3dTools::GetDirection_up(_currentQuaternion);
		Vector3 targetRight = My3dTools::GetDirection_right(_currentQuaternion);
		// 跟随的目标的位置，并且加上自定义的位置偏移
		_targetPos = targetWorldTransform.translation_ + targetFront * _posOffset.z + targetUp * _posOffset.y + targetRight * _posOffset.x;

		if (_isStartAni) {
			Vector3 dir = _targetPos - _pos;
			if (dir.Length() > 1) {
				_pos += dir.Normalize() * _speed_startAni;
			} else {
				_isStart = true;
				_isStartAni = false;
			}
		}
		if (_isStart) {
			// 第一人称和第三人称变换
			if (Input::GetInstance()->TriggerKey(DIK_R))
				_is1rd = !_is1rd;
			if (_is1rd) {
				_posOffset = _posOffset_1rd;
				_rotateOffset = _rotateOffset_1rd;
			} else {
				_posOffset = _posOffset_3rd;
				_rotateOffset = _rotateOffset_3rd;
			}

			_pos = _targetPos;
			_currentQuaternion = _rotateOffset * _target->GetQuaternion(); // 加上旋转偏移
		}
	}

	// 重新传入摄像机的view矩阵中
	Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _currentQuaternion, _pos);
	// Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _rotate, _pos);
	_viewProjection.matView = m.Inverse();
	_viewProjection.TransferMatrix();
}
