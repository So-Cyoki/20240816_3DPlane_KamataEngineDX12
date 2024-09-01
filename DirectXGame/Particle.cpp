#include "Particle.h"

void Particle::Move() {
	switch (_type) {
	case Particle::tHurt:
	case Particle::tBoom2:
	case Particle::tMove: {
		if (!FrameTimeWatch(_lifeTime, 1)) {
			float t = _currentTimes[1] / (float)_lifeTime;
			_pos += _moveDir * _speed * (1 - EaseOutQuart(t));
			_scale = _targetScale * (1 - EaseOutQuart(t));
			Quaternion rotate = {1, 0.1f, 0, 0.1f};
			_currentQuaternion = rotate * _currentQuaternion;
		}
		break;
	}
	case Particle::tBoom: {
		if (!FrameTimeWatch(_lifeTime, 1)) {
			float t = _currentTimes[1] / (float)_lifeTime;
			_scale = _targetScale * EaseOutBack(t);
			Quaternion rotate = {1, 0.01f, 0.01f, 0.01f};
			_currentQuaternion = rotate * _currentQuaternion;
		}
		break;
	}
	case Particle::tBoom3: {
		if (!FrameTimeWatch(_lifeTime, 1)) {
			float t = _currentTimes[1] / (float)_lifeTime;
			_scale = _targetScale * EaseOutBack(t);
			Quaternion rotate = {1, 0.02f, 0.02f, 0.02f};
			_currentQuaternion = rotate * _currentQuaternion;
		}
		break;
	}
	}
}

void Particle::ToDead() {
	if (!FrameTimeWatch(_deadTime, 2)) {
		float t = _currentTimes[2] / (float)_deadTime;
		_model->SetAlpha(1 - EaseInSine(t));

		//_pos += _moveDir * _speed * (1 - EaseOutQuart(t));
		//// Quaternion rotate = {1, 0.1f, 0, 0.1f};
		////_currentQuaternion = rotate * _currentQuaternion;
		//_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
		//_worldTransform.TransferMatrix();
	} else {
		ParticleManager::ReleaseParticle(this);
	}
}

bool Particle::FrameTimeWatch(int frame, int index) {
	if (_currentTimes[index] > frame) {
		_currentTimes[index] = 0;
		return true;
	}
	_currentTimes[index]++;
	return false;
}

void Particle::Initalize(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Type type) {
	_viewProjection = viewProjection;
	_worldTransform.Initialize();
	_worldTransform.translation_ = position;
	_pos = position;
	_worldTransform.scale_ = _scale;
	_currentQuaternion = rotate;
	_type = type;
	_model->SetAlpha(1);
	_isDead = false;

	switch (_type) {
	case Particle::tHurt: {
		float scale = 1.f;
		_targetScale = {scale, scale, scale};
		_speed = 0.5f;
		_lifeTime = 1 * 60;
		_deadTime = 0 * 60;

		float randRadian = 15 * std::acosf(-1) / 180; // 方向随机轴转一下，更自然
		Quaternion randX = My3dTools::RandomRotation(randRadian, 0);
		Quaternion randY = My3dTools::RandomRotation(randRadian, 1);
		_currentQuaternion = randX * _currentQuaternion;
		_currentQuaternion = randY * _currentQuaternion;

		_moveDir = My3dTools::GetDirection_front(_currentQuaternion) * -1;
		break;
	}
	case Particle::tBoom: {
		float scale = 5.f;
		_targetScale = {scale, scale, scale};
		_lifeTime = 1 * 60;
		_deadTime = 1 * 60;
		break;
	}
	case Particle::tBoom2: {
		float scale = 3.f;
		_targetScale = {scale, scale, scale};
		_speed = 0.5f;
		_lifeTime = 1 * 60;
		_deadTime = 0 * 60;

		float randRadian = 15 * std::acosf(-1) / 180; // 方向随机轴转一下，更自然
		Quaternion randX = My3dTools::RandomRotation(randRadian, 0);
		Quaternion randY = My3dTools::RandomRotation(randRadian, 1);
		_currentQuaternion = randX * _currentQuaternion;
		_currentQuaternion = randY * _currentQuaternion;

		_moveDir = My3dTools::GetDirection_front(_currentQuaternion) * -1;
		break;
	}
	case Particle::tBoom3: {
		float scale = 12.f;
		_targetScale = {scale, scale, scale};
		_lifeTime = 3 * 60;
		_deadTime = int(0.5f * 60);
		break;
	}
	case Particle::tMove: {
		float scale = 1.7f;
		_targetScale = {scale, scale, scale};
		_speed = 0.1f;
		_lifeTime = 2 * 60;
		_deadTime = 0 * 60;
		_moveDir = My3dTools::GetDirection_front(_currentQuaternion) * -1;
		break;
	}
	}
	_scale = _targetScale;
}

void Particle::Update() {
	if (FrameTimeWatch(_lifeTime, 0)) {
		_isDead = true;
	} else {
		Move();
	}

	_worldTransform.scale_ = _scale;
	_worldTransform.translation_ = _pos;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
}

void Particle::Draw() {
	switch (_type) {
	case Particle::tHurt:
	case Particle::tBoom2:
		_model->Draw(_worldTransform, *_viewProjection, _textureHandle_hurt);
		break;
	case Particle::tBoom:
	case Particle::tBoom3:
	case Particle::tMove:
		_model->Draw(_worldTransform, *_viewProjection, _textureHandle_boom);
		break;
	}
}

void Particle::Fire() { ParticleManager::_updatePool.push_back(this); }

const Vector3 Particle::GetWorldPosition() const {
	Vector3 worldPos{};
	worldPos.x = _worldTransform.matWorld_.m[3][0];
	worldPos.y = _worldTransform.matWorld_.m[3][1];
	worldPos.z = _worldTransform.matWorld_.m[3][2];
	return worldPos;
}

void ParticleManager::ADD_Hurt(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate) {
	Particle* p1 = AcquireParticle(viewProjection, position, rotate, Particle::tHurt);
	p1->Fire();
	// Particle* p2 = AcquireParticle(viewProjection, position, rotate, Particle::tHurt);
	// p2->Fire();
	// Particle* p3 = AcquireParticle(viewProjection, position, rotate, Particle::tHurt);
	// p3->Fire();
}

void ParticleManager::ADD_Boom(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate) {
	Particle* p1 = AcquireParticle(viewProjection, position, rotate, Particle::tBoom);
	p1->Fire();
}

void ParticleManager::ADD_Boom2(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate) {
	Particle* p1 = AcquireParticle(viewProjection, position, rotate, Particle::tBoom2);
	p1->Fire();
	Particle* p2 = AcquireParticle(viewProjection, position, rotate, Particle::tBoom2);
	p2->Fire();
}

void ParticleManager::ADD_Boom3(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate) {
	Particle* p1 = AcquireParticle(viewProjection, position, rotate, Particle::tBoom3);
	p1->Fire();
}

void ParticleManager::ADD_Move(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate) {
	Particle* p1 = AcquireParticle(viewProjection, position, rotate, Particle::tMove);
	p1->Fire();
}

void ParticleManager::Updata() {
	for (Particle* it : ParticleManager::_updatePool) {
		if (!it->GetIsDead())
			it->Update();
		else
			it->ToDead();
	}
}

void ParticleManager::Draw() {
	for (Particle* it : ParticleManager::_updatePool) {
		it->Draw();
	}
}

Particle* ParticleManager::AcquireParticle(ViewProjection* viewProjection, const Vector3& position, const Quaternion& rotate, Particle::Type type) {
	if (_idlePool.empty()) {
		Particle* particle = new Particle();
		particle->Initalize(viewProjection, position, rotate, type);
		return particle;
	} else {
		Particle* particle = _idlePool.front();
		_idlePool.pop();
		particle->Initalize(viewProjection, position, rotate, type);
		return particle;
	}
}

void ParticleManager::ReleaseParticle(Particle* particle) {
	auto it = std::find(_updatePool.begin(), _updatePool.end(), particle);
	if (it != _updatePool.end()) {
		std::swap(*it, _updatePool.back());
		_updatePool.pop_back();
		_idlePool.push(particle);
	}
}
