#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete _model;
	delete _cameraConObj;
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
	_enemyObj = new Enemy();
	Vector3 enemyPos = {0, 0, 0};
	_enemyObj->Initalize(&_viewProjection, enemyPos);
}

void GameScene::Update() {
	// DebugText
	ImGui::Begin("DeBug Window");
	ImGui::DragFloat3("Camera Translate", &_cameraConObj->_cameraPos.x, 0.01f);
	ImGui::DragFloat3("Camera Rotate", &_cameraConObj->_cameraRotate.x, 0.01f);
	ImGui::End();
	// CameraController
	_cameraConObj->Update();
	_viewProjection.matView = _cameraConObj->GetViewProjection().matView;
	_viewProjection.matProjection = _cameraConObj->GetViewProjection().matProjection;
	_viewProjection.TransferMatrix();
	// Obj
	_enemyObj->Update();
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
