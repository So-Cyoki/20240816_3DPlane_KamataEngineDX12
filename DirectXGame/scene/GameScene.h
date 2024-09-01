#pragma once

#include "Audio.h"
#include "Bullet.h"
#include "CameraController.h"
#include "DirectXCommon.h"
#include "EarthBall.h"
#include "Enemy.h"
#include "GameUI.h"
#include "Input.h"
#include "Model.h"
#include "My3DTools.h"
#include "Particle.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#ifdef _DEBUG
#include <imgui.h>
#endif

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 自分の変数
	// FrontObj
	ViewProjection _viewProjection;
	Model* _model = nullptr;
	CameraController* _cameraConObj = nullptr;
	// Obj
	Skydome* _skydomeObj = nullptr;
	Player* _playerObj = nullptr;
	EarthBall* _earthBall = nullptr;
	Vector3 _playerBornPos = {0, 50, -200};
	Vector3 _cameraConBornPos = {0, 20, -20};
	Vector3 _cameraConBornRotate = {0.4f, 0, 0};
	// UI
	GameUI* _gameUIObj = nullptr;
	DeadUI* _deadUIObj = nullptr;
	StartUI* _startUIObj = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
