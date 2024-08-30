#include "Particle.h"

void Particle::Move() {
	Vector3 front = My3dTools::GetDirection_front(_currentQuaternion);
	switch (_type) {
	case Particle::tHurt:
		front *= -1; // 不知道要怎么翻转四元数，所以翻转前进方向好了
		break;
	case Particle::tBoom:
		break;
	}
	_pos += front * _speed;
}

void Particle::ToDead() { ParticleManager::ReleaseParticle(this); }

bool Particle::FrameTimeWatch_false(int frame, int index) {
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

	switch (_type) {
	case Particle::tHurt: {
		_scale = {0.3f, 0.3f, 0.3f};
		float randRadian = 10 * std::acosf(-1) / 180; // 随机轴转一下，更自然
		Quaternion randX = My3dTools::RandomRotation(randRadian, 0);
		Quaternion randY = My3dTools::RandomRotation(randRadian, 1);
		_currentQuaternion = randX * _currentQuaternion;
		_currentQuaternion = randY * _currentQuaternion;
		break;
	}
	case Particle::tBoom: {

		break;
	}
	}

	_isDead = false;
}

void Particle::Update() {
	if (FrameTimeWatch_false(_liftTime, 0)) {
		_isDead = true;
	}
	Move();

	_worldTransform.scale_ = _scale;
	_worldTransform.translation_ = _pos;
	_worldTransform.matWorld_ = Matrix4x4::MakeAffineMatrix(_worldTransform.scale_, _currentQuaternion, _worldTransform.translation_);
	_worldTransform.TransferMatrix();
}

void Particle::Draw() { _model->Draw(_worldTransform, *_viewProjection, _textureHandle); }

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
	Particle* p2 = AcquireParticle(viewProjection, position, rotate, Particle::tHurt);
	p2->Fire();
	Particle* p3 = AcquireParticle(viewProjection, position, rotate, Particle::tHurt);
	p3->Fire();
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
