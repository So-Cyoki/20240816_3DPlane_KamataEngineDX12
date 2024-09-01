#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _model;
	delete _cameraConObj;
	delete _playerObj;
	delete _skydomeObj;
	delete _gameUIObj;
	delete _deadUIObj;
	delete _startUIObj;
	for (Bullet* it : BulletManager::_updatePool_player)
		delete it;
	BulletManager::_updatePool_player.clear();
	for (Bullet* it : BulletManager::_updatePool_enemy)
		delete it;
	BulletManager::_updatePool_enemy.clear();
	for (Enemy* it : EnemyManager::_updatePool)
		delete it;
	EnemyManager::_updatePool.clear();
	for (Particle* it : ParticleManager::_updatePool)
		delete it;
	ParticleManager::_updatePool.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// FrontObj
	_model = Model::Create();     // TempModel
	_viewProjection.Initialize(); // ViewProjection
	_cameraConObj = new CameraController();
	_cameraConObj->Initialize();
	// Obj
	_skydomeObj = new Skydome();
	_skydomeObj->Initialize(&_viewProjection);
	_playerObj = new Player();
	_playerObj->Initalize(&_viewProjection, _playerBornPos);
	_cameraConObj->SetTarget(_playerObj);
	_earthBall = new EarthBall;
	_earthBall->Initialize(&_viewProjection, _playerObj);

	// UI
	_gameUIObj = new GameUI();
	_gameUIObj->Initalize(WinApp::kWindowWidth, WinApp::kWindowHeight, _playerObj);
	_deadUIObj = new DeadUI();
	_deadUIObj->Initalize(WinApp::kWindowWidth, WinApp::kWindowHeight);
	_startUIObj = new StartUI();
	_startUIObj->Initalize(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 预先生成并存放子弹，为了优化
	// for (int i = 0; i < 2000; i++) {
	//	Bullet* bullet = new Bullet();
	//	BulletManager::_idlePool.push(bullet);
	//}
	// for (int i = 0; i < 5000; i++) {
	//	Particle* par = new Particle();
	//	ParticleManager::_idlePool.push(par);
	//}
}

void GameScene::Update() {
	switch (UIManager::_currentScene) {
	case UIManager::Scene::Loading: {
		// Camera
		_cameraConObj->Initialize();
		_cameraConObj->SetIsStartAni(true);
		// Player
		_playerObj->Initalize(&_viewProjection, _playerBornPos);
		EnemyManager::_isStart = false;
		// Enemy
		std::vector<Enemy*> tempEnemy;
		for (Enemy* it : EnemyManager::_updatePool)
			tempEnemy.push_back(it);
		for (Enemy* it : tempEnemy)
			EnemyManager::ReleaseEnemy(it);
		EnemyManager::_updatePool.clear();
		tempEnemy.clear();
		// Bullet
		std::vector<Bullet*> tempBullet;
		for (Bullet* it : BulletManager::_updatePool_player)
			tempBullet.push_back(it);
		for (Bullet* it : BulletManager::_updatePool_enemy)
			tempBullet.push_back(it);
		for (Bullet* it : tempBullet)
			BulletManager::ReleaseBullet(it);
		BulletManager::_updatePool_player.clear();
		BulletManager::_updatePool_enemy.clear();
		tempBullet.clear();
		// Particle
		std::vector<Particle*> tempParticle;
		for (Particle* it : ParticleManager::_updatePool)
			tempParticle.push_back(it);
		for (Particle* it : tempParticle)
			ParticleManager::ReleaseParticle(it);
		ParticleManager::_updatePool.clear();
		tempParticle.clear();
		// UI
		_gameUIObj->SetScore(0);
		_deadUIObj->SetIsRestart(false);
		_startUIObj->SetIsStart(false);
		_gameUIObj->SetColor_killPoint({1, 1, 1, 0});
		_gameUIObj->SetColorHelp({1, 1, 1, 1});
		// Next
		UIManager::_currentScene = UIManager::Scene::Game;
		break;
	}
	case UIManager::Scene::Start: {
		// CameraController
		_cameraConObj->Update();
		_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
		_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
		_viewProjection.TransferMatrix();
		// Obj
		_skydomeObj->Update();
		_earthBall->Update();
		// UI
		_startUIObj->Update();
		// Next
		if (_startUIObj->GetIsStart()) {
			_cameraConObj->SetIsStartAni(true);
			if (_cameraConObj->GetIsStart())
				UIManager::_currentScene = UIManager::Scene::Loading;
		}
		break;
	}
	case UIManager::Scene::Game: {
		// CameraController
		_cameraConObj->Update();
		_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
		_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
		_viewProjection.TransferMatrix();
		// Obj
		_skydomeObj->Update();
		if (_cameraConObj->GetIsStart())
			_playerObj->Update();
		_earthBall->Update();
		EnemyManager::EnemyBornSystem(&_viewProjection, _playerObj, _gameUIObj);
		EnemyManager::Updata();
		BulletManager::Updata();
		ParticleManager::Updata();
		// UI
		_gameUIObj->Update();
		// Next
		if (_playerObj->GetIsDead())
			UIManager::_currentScene = UIManager::Scene::Dead;
		break;
	}
	case UIManager::Scene::Dead: {
		// CameraController
		_cameraConObj->Update();
		_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
		_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
		_viewProjection.TransferMatrix();
		// Obj
		_skydomeObj->Update();
		_earthBall->Update();
		EnemyManager::Updata();
		BulletManager::Updata();
		ParticleManager::Updata();
		// UI
		_deadUIObj->Update();
		// Next
		if (_deadUIObj->GetIsRestart()) {
			_cameraConObj->SetCameraPos(_playerBornPos);
			UIManager::_currentScene = UIManager::Scene::Loading;
		}
		break;
	}
	}

#ifdef _DEBUG
	// DebugText
	ImGui::Begin("DeBug Window");
	ImGui::DragFloat3("Camera Translate", (float*)&_cameraConObj->GetCameraPos(), 0.01f);
	// ImGui::DragFloat3("Camera Rotate", (float*)&_cameraConObj->GetCameraRotate(), 0.01f);
	ImGui::DragFloat3("Player Translate", (float*)&_playerObj->GetPos(), 0.01f);
	ImGui::DragFloat3("Player Rotate", (float*)&_playerObj->GetRotate(), 0.01f);
	ImGui::Text("Enemy Value = %d", EnemyManager::_updatePool.size());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("Player Acc (%f,%f,%f)", _playerObj->GetAccelerations().x, _playerObj->GetAccelerations().y, _playerObj->GetAccelerations().z);
	ImGui::Text("Player Vel (%f,%f,%f)", _playerObj->GetVelocity().x, _playerObj->GetVelocity().y, _playerObj->GetVelocity().z);
	ImGui::Text("Player GasVel %f", _playerObj->GetMoveGasPedal());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("Bullet Player = %d", int(BulletManager::_updatePool_player.size()));
	ImGui::Text("Bullet Enemy = %d", int(BulletManager::_updatePool_enemy.size()));
	ImGui::Text("Bullet Idle = %d", int(BulletManager::_idlePool.size()));
	ImGui::Text("Particle update = %d", int(ParticleManager::_updatePool.size()));
	ImGui::Text("Particle Idle = %d", int(ParticleManager::_idlePool.size()));
	// ImGui::DragFloat3("Camera Pos Offset", (float*)&_cameraConObj->_posOffset, 0.01f);
	// ImGui::DragFloat3("Camera Rotate Offset", (float*)&_cameraConObj->_rotateOffset, 0.01f);
	ImGui::End();
#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	_skydomeObj->Draw();
	_earthBall->Draw();
	_playerObj->Draw();
	EnemyManager::Draw();
	BulletManager::Draw();
	ParticleManager::Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	switch (UIManager::_currentScene) {
	case UIManager::Scene::Start:
		_startUIObj->Draw();
		break;
	case UIManager::Scene::Game:
		_gameUIObj->Draw();
		break;
	case UIManager::Scene::Dead:
		_deadUIObj->Draw();
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
