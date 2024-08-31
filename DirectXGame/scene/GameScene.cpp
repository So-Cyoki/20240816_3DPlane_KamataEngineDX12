#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _model;
	delete _cameraConObj;
	delete _playerObj;
	delete _enemyObj;
	delete _skydomeObj;
	delete _gameUIObj;
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
	Vector3 playerPos = {0, 0, -200};
	_playerObj->Initalize(&_viewProjection, playerPos);
	_cameraConObj->SetTarget(_playerObj);
	_earthBall = new EarthBall;
	_earthBall->Initialize(&_viewProjection, _playerObj);

	_enemyObj = new Enemy();
	Vector3 enemyPos = {0, 0, -100};
	_enemyObj->Initalize(&_viewProjection, enemyPos, _playerObj);
	const int newEnemy = 0;
	for (int i = 0; i < newEnemy; i++) {
		Enemy* enemy = EnemyManager::AcquireEnemy(&_viewProjection, {20.f * i, 20, -20}, _playerObj);
		enemy->Fire();
	}
	for (int i = 0; i < newEnemy; i++) {
		Enemy* enemy = EnemyManager::AcquireEnemy(&_viewProjection, {-20.f * i, 20, -20}, _playerObj);
		enemy->Fire();
	}
	// 预先生成并存放子弹，为了优化
	// for (int i = 0; i < 2000; i++) {
	//	Bullet* bullet = new Bullet();
	//	BulletManager::_idlePool.push(bullet);
	//}
	// for (int i = 0; i < 5000; i++) {
	//	Particle* par = new Particle();
	//	ParticleManager::_idlePool.push(par);
	//}

	// UI
	_gameUIObj = new GameUI();
	_gameUIObj->Initalize(WinApp::kWindowWidth, WinApp::kWindowHeight, _playerObj);
}

void GameScene::Update() {
	// CameraController
	_cameraConObj->Update();
	_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
	_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
	_viewProjection.TransferMatrix();
	// Obj
	_skydomeObj->Update();
	_playerObj->Update();
	_enemyObj->Update();
	_earthBall->Update();
	EnemyManager::Updata();
	BulletManager::Updata();
	ParticleManager::Updata();
	// UI
	_gameUIObj->Update();

#ifdef _DEBUG
	// Collision
	bool isCollision = My3dTools::IsCollision(_playerObj->GetSphere(), _enemyObj->GetSphere());
	// DebugText
	ImGui::Begin("DeBug Window");
	// ImGui::DragFloat3("Camera Translate", (float*)&_cameraConObj->GetCameraPos(), 0.01f);
	// ImGui::DragFloat3("Camera Rotate", (float*)&_cameraConObj->GetCameraRotate(), 0.01f);
	ImGui::DragFloat3("Player Translate", (float*)&_playerObj->GetPos(), 0.01f);
	ImGui::DragFloat3("Player Rotate", (float*)&_playerObj->GetRotate(), 0.01f);
	ImGui::DragFloat3("Enemy Translate", (float*)&_enemyObj->GetPos(), 0.01f);
	// ImGui::DragFloat3("Enemy Rotate", (float*)&_enemyObj->GetRotate(), 0.01f);
	ImGui::DragFloat4("Enemy Quaternion", (float*)&_enemyObj->_currentQuaternion, 0.01f);
	ImGui::Text("EnemyHp = %f", _enemyObj->GetHp());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("IsCollision %s", isCollision ? "true" : "false");
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
	_enemyObj->Draw();
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

	_gameUIObj->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
