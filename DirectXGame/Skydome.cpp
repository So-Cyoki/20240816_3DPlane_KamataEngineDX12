#include "Skydome.h"

Skydome::~Skydome() { delete _model; }

void Skydome::Initialize(ViewProjection* view) {
	_worldTransform.Initialize();
	_viewProjection = view;
	_model = Model::CreateFromOBJ("Skydome", true);

	_worldTransform.scale_ = _scale;
	_worldTransform.rotation_ = {0, 60 * acosf(-1) / 180.f, 0};
}

void Skydome::Update() { _worldTransform.UpdateMatrix(); }

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
