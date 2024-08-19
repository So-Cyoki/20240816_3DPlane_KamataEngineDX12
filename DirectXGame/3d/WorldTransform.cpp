#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = Matrix4x4::MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}