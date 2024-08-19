#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _model;
	delete _cameraConObj;
	delete _playerObj;
	delete _enemyObj;
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
	_playerObj = new Player();
	Vector3 playerPos = {0, 0, -70};
	_playerObj->Initalize(&_viewProjection, playerPos);
	_cameraConObj->SetTarget(_playerObj);
	_enemyObj = new Enemy();
	Vector3 enemyPos = {0, 0, 0};
	_enemyObj->Initalize(&_viewProjection, enemyPos);
}

void GameScene::Update() {
	// CameraController
	_cameraConObj->Update();
	_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
	_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
	_viewProjection.TransferMatrix();
	// Obj
	_playerObj->Update();
	_enemyObj->Update();
	// Collision
	bool isCollision = My3dTools::IsCollision(_playerObj->GetSphere(), _enemyObj->GetSphere());

	// DebugText
	ImGui::Begin("DeBug Window");
	ImGui::DragFloat3("Camera Translate", (float*)&_cameraConObj->GetCameraPos(), 0.01f);
	ImGui::DragFloat3("Camera Rotate", (float*)&_cameraConObj->GetCameraRotate(), 0.01f);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("IsCollision %s", isCollision ? "true" : "false");
	ImGui::Text("Player Acc (%f,%f,%f)", _playerObj->GetAccelerations().x, _playerObj->GetAccelerations().y, _playerObj->GetAccelerations().z);
	ImGui::Text("Player Vel (%f,%f,%f)", _playerObj->GetVelocity().x, _playerObj->GetVelocity().y, _playerObj->GetVelocity().z);
	ImGui::End();
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

	_playerObj->Draw();
	_enemyObj->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
