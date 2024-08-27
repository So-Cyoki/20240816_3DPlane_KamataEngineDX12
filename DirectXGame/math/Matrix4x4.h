#pragma once
#include "Quaternion.h"
#include "Vector3.h"
#include <cmath>
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

public:
	// 4x4行列の掛け算
	static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2) {
		Matrix4x4 result{};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
			}
		}
		return result;
	};
	// 4x4行列のアフィン変換
	static Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotation, Vector3 translation) {
		// Scale
		Matrix4x4 mScale = {0};
		mScale.m[0][0] = scale.x;
		mScale.m[1][1] = scale.y;
		mScale.m[2][2] = scale.z;
		mScale.m[3][3] = 1;
		// Rotation
		// Matrix4x4 mRotationZ = {0};
		// mRotationZ.m[0][0] = cosf(rotation.z);
		// mRotationZ.m[0][1] = sinf(rotation.z);
		// mRotationZ.m[1][0] = -sinf(rotation.z);
		// mRotationZ.m[1][1] = cosf(rotation.z);
		// mRotationZ.m[2][2] = mRotationZ.m[3][3] = 1;
		// Matrix4x4 mRotationX = {0};
		// mRotationX.m[1][1] = cosf(rotation.x);
		// mRotationX.m[1][2] = sinf(rotation.x);
		// mRotationX.m[2][1] = -sinf(rotation.x);
		// mRotationX.m[2][2] = cosf(rotation.x);
		// mRotationX.m[0][0] = mRotationX.m[3][3] = 1;
		// Matrix4x4 mRotationY = {0};
		// mRotationY.m[0][0] = cosf(rotation.y);
		// mRotationY.m[2][0] = sinf(rotation.y);
		// mRotationY.m[0][2] = -sinf(rotation.y);
		// mRotationY.m[2][2] = cosf(rotation.y);
		// mRotationY.m[1][1] = mRotationY.m[3][3] = 1;
		// Matrix4x4 mRotation = Multiply(mRotationX, Multiply(mRotationY, mRotationZ));
		Quaternion quatRotation = Quaternion::RadianToQuaternion(rotation);
		Matrix4x4 mRotation = Matrix4x4::ToMatrix4x4(quatRotation);
		//  Translation
		Matrix4x4 mTranslation = {0};
		mTranslation.m[0][0] = mTranslation.m[1][1] = mTranslation.m[2][2] = mTranslation.m[3][3] = 1;
		mTranslation.m[3][0] = translation.x;
		mTranslation.m[3][1] = translation.y;
		mTranslation.m[3][2] = translation.z;

		return Multiply(mScale, Multiply(mRotation, mTranslation));
	};
	static Matrix4x4 MakeAffineMatrix(Vector3 scale, Quaternion quatRotation, Vector3 translation) {
		// Scale
		Matrix4x4 mScale = {0};
		mScale.m[0][0] = scale.x;
		mScale.m[1][1] = scale.y;
		mScale.m[2][2] = scale.z;
		mScale.m[3][3] = 1;
		// Rotation
		Matrix4x4 mRotation = Matrix4x4::ToMatrix4x4(quatRotation);
		//  Translation
		Matrix4x4 mTranslation = {0};
		mTranslation.m[0][0] = mTranslation.m[1][1] = mTranslation.m[2][2] = mTranslation.m[3][3] = 1;
		mTranslation.m[3][0] = translation.x;
		mTranslation.m[3][1] = translation.y;
		mTranslation.m[3][2] = translation.z;

		return Multiply(mScale, Multiply(mRotation, mTranslation));
	};
	// Quaternion to Matrix4x4
	static Matrix4x4 ToMatrix4x4(const Quaternion& q) {
		Matrix4x4 matrix = {0};
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
		matrix.m[0][1] = 2.0f * (xy + wz);
		matrix.m[0][2] = 2.0f * (xz - wy);
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = 2.0f * (xy - wz);
		matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
		matrix.m[1][2] = 2.0f * (yz + wx);
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = 2.0f * (xz + wy);
		matrix.m[2][1] = 2.0f * (yz - wx);
		matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);
		matrix.m[2][3] = 0.0f;

		// matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
		// matrix.m[0][1] = 2.0f * (xy - wz);
		// matrix.m[0][2] = 2.0f * (xz + wy);
		// matrix.m[0][3] = 0.0f;

		// matrix.m[1][0] = 2.0f * (xy + wz);
		// matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
		// matrix.m[1][2] = 2.0f * (yz - wx);
		// matrix.m[1][3] = 0.0f;

		// matrix.m[2][0] = 2.0f * (xz - wy);
		// matrix.m[2][1] = 2.0f * (yz + wx);
		// matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);
		// matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = 0.0f;
		matrix.m[3][1] = 0.0f;
		matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	// 逆行列
	Matrix4x4 Inverse() const {
		Matrix4x4 result{};
		float detA = m[0][0] * m[1][1] * m[2][2] * m[3][3] + m[0][0] * m[1][2] * m[2][3] * m[3][1] + m[0][0] * m[1][3] * m[2][1] * m[3][2] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
		             m[0][0] * m[1][2] * m[2][1] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2] - m[0][1] * m[1][0] * m[2][2] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1] -
		             m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][3] * m[1][0] * m[2][2] * m[3][1] + m[0][2] * m[1][0] * m[2][1] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2] +
		             m[0][1] * m[1][2] * m[2][0] * m[3][3] + m[0][2] * m[1][3] * m[2][0] * m[3][1] + m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][3] * m[1][2] * m[2][0] * m[3][1] -
		             m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][1] * m[1][3] * m[2][0] * m[3][2] - m[0][1] * m[1][2] * m[2][3] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
		             m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][3] * m[1][2] * m[2][1] * m[3][0] + m[0][2] * m[1][1] * m[2][3] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0];

		float adjA[4][4] = {0};
		adjA[0][0] = m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][3] * m[2][2] * m[3][1] - m[1][2] * m[2][1] * m[3][3] - m[1][1] * m[2][3] * m[3][2];
		adjA[0][1] = -m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2] + m[0][3] * m[2][2] * m[3][1] + m[0][2] * m[2][1] * m[3][3] + m[0][1] * m[2][3] * m[3][2];
		adjA[0][2] = m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][3] * m[1][2] * m[3][1] - m[0][2] * m[1][1] * m[3][3] - m[0][1] * m[1][3] * m[3][2];
		adjA[0][3] = -m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2] + m[0][3] * m[1][2] * m[2][1] + m[0][2] * m[1][1] * m[2][3] + m[0][1] * m[1][3] * m[2][2];
		adjA[1][0] = -m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2] + m[1][3] * m[2][2] * m[3][0] + m[1][2] * m[2][0] * m[3][3] + m[1][0] * m[2][3] * m[3][2];
		adjA[1][1] = m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][3] * m[2][2] * m[3][0] - m[0][2] * m[2][0] * m[3][3] - m[0][0] * m[2][3] * m[3][2];
		adjA[1][2] = -m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2] + m[0][3] * m[1][2] * m[3][0] + m[0][2] * m[1][0] * m[3][3] + m[0][0] * m[1][3] * m[3][2];
		adjA[1][3] = m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][3] * m[1][2] * m[2][0] - m[0][2] * m[1][0] * m[2][3] - m[0][0] * m[1][3] * m[2][2];
		adjA[2][0] = m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][3] * m[2][1] * m[3][0] - m[1][1] * m[2][0] * m[3][3] - m[1][0] * m[2][3] * m[3][1];
		adjA[2][1] = -m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1] + m[0][3] * m[2][1] * m[3][0] + m[0][1] * m[2][0] * m[3][3] + m[0][0] * m[2][3] * m[3][1];
		adjA[2][2] = m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][3] * m[1][1] * m[3][0] - m[0][1] * m[1][0] * m[3][3] - m[0][0] * m[1][3] * m[3][1];
		adjA[2][3] = -m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1] + m[0][3] * m[1][1] * m[2][0] + m[0][1] * m[1][0] * m[2][3] + m[0][0] * m[1][3] * m[2][1];
		adjA[3][0] = -m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1] + m[1][2] * m[2][1] * m[3][0] + m[1][1] * m[2][0] * m[3][2] + m[1][0] * m[2][2] * m[3][1];
		adjA[3][1] = m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][2] * m[2][1] * m[3][0] - m[0][1] * m[2][0] * m[3][2] - m[0][0] * m[2][2] * m[3][1];
		adjA[3][2] = -m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1] + m[0][2] * m[1][1] * m[3][0] + m[0][1] * m[1][0] * m[3][2] + m[0][0] * m[1][2] * m[3][1];
		adjA[3][3] = m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1];

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 1 / detA * adjA[i][j];
			}
		}
		return result;
	};
};