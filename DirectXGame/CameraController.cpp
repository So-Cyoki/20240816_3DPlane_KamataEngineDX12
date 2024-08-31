#include "CameraController.h"

void CameraController::Attack() {
	// 攻击计时
	bool isAttack = false;
	if (_currentTime <= 0) {
		_currentTime = _attackTime;
		isAttack = true;
	}
	_currentTime--;
	// 攻击
	if (Input::GetInstance()->IsPressMouse(0) && isAttack) {
		Vector3 offset = {1, 1, 2}; // 调整子弹的出现位置

		Vector3 up = My3dTools::GetDirection_up(_currentQuaternion);
		Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
		Vector3 right = My3dTools::GetDirection_right(_currentQuaternion);
		Vector3 bulletBornPos1 = _pos + up * offset.y + front * offset.z + right * offset.x;
		Vector3 bulletBornPos2 = _pos + up * offset.y + front * offset.z + right * -offset.x;
		Vector3 bulletBornRotate = {_rotate.x, _rotate.y, 0};
		Bullet* bullet1 = BulletManager::AcquireBullet(&_viewProjection, bulletBornPos1, _currentQuaternion, Bullet::tPlayer);
		bullet1->Fire();
		Bullet* bullet2 = BulletManager::AcquireBullet(&_viewProjection, bulletBornPos2, _currentQuaternion, Bullet::tPlayer);
		bullet2->Fire();
	}
}

void CameraController::Initialize() {
	_viewProjection.Initialize();
	_currentQuaternion = {1, 0, 0, 0};
	_beforeRotate = {0, 0, 0};

	_pos = {0, 20, -170};
	_rotate = {0.4f, 0, 0};
}

void CameraController::Update() {
	if (_target != nullptr) {
		// Attack();

		// 获取目标的位置和旋转信息
		const WorldTransform& targetWorldTransform = _target->GetWorldTransform();

		// Vector3 targetFront = My3dTools::GetDirection_front(_target->GetQuaternion());
		// Vector3 targetUp = My3dTools::GetDirection_up(_target->GetQuaternion());
		// Vector3 targetRight = My3dTools::GetDirection_right(_target->GetQuaternion());
		Vector3 targetFront = My3dTools::GetDirection_front(_currentQuaternion);
		Vector3 targetUp = My3dTools::GetDirection_up(_currentQuaternion);
		Vector3 targetRight = My3dTools::GetDirection_right(_currentQuaternion);
		// 跟随的目标的位置，并且加上自定义的位置偏移
		_targetPos = targetWorldTransform.translation_ + targetFront * _posOffset.z + targetUp * _posOffset.y + targetRight * _posOffset.x;
		_targetRotate = targetWorldTransform.rotation_ + _rotateOffset;

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
	_currentQuaternion.normalize();
	Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _currentQuaternion, _pos);
	// Matrix4x4 m = Matrix4x4::MakeAffineMatrix({1, 1, 1}, _rotate, _pos);
	_viewProjection.matView = m.Inverse();
	_viewProjection.TransferMatrix();
	_beforeRotate = _rotate;
}
