#include "GameUI.h"

Sprite* GameUI::MyCreate(const std::string& fileName) {
	Sprite* sp = nullptr;
	_textureHandle = TextureManager::Load(fileName);
	sp = Sprite::Create(_textureHandle, {-100, -100});
	sp->SetAnchorPoint({0.5f, 0.5f});
	return sp;
}

GameUI::~GameUI() {
	for (Sprite* it : _spList)
		delete it;
	_spList.clear();
}

void GameUI::Initalize(int screenWidth, int screenHeight, Player* playerObj) {
	_width = float(screenWidth), _height = float(screenHeight);
	_playerObj = playerObj;

	_spArrow2 = MyCreate("Sprites/arrow_mouse2.png");
	_spArrow2->SetPosition({_width / 2, _height / 2});
	_spList.push_back(_spArrow2);
	_spArrow1 = MyCreate("Sprites/arrow_mouse.png");
	_spArrow1->SetPosition({_width / 2, _height / 2});
	_spList.push_back(_spArrow1);
}

void GameUI::Update() {

	// Player的两个瞄准UI
	_spArrow1->SetPosition(_playerObj->GetArrowMouse());
	_spArrow2->SetPosition(_playerObj->GetArrowMove());
}

void GameUI::Draw() {
	for (Sprite* it : _spList)
		it->Draw();
}
