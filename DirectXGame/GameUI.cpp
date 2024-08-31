#include "GameUI.h"

void GameUI::SpriteHp_Initalize() {
	_textureHandle = TextureManager::Load("Sprites/Hp/hp01.png");
	_spHpArray[0] = Sprite::Create(_textureHandle, {861, 399});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp02.png");
	_spHpArray[1] = Sprite::Create(_textureHandle, {857, 412});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp03.png");
	_spHpArray[2] = Sprite::Create(_textureHandle, {854, 423});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp04.png");
	_spHpArray[3] = Sprite::Create(_textureHandle, {849, 434});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp05.png");
	_spHpArray[4] = Sprite::Create(_textureHandle, {845, 445});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp06.png");
	_spHpArray[5] = Sprite::Create(_textureHandle, {839, 456});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp07.png");
	_spHpArray[6] = Sprite::Create(_textureHandle, {833, 467});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp08.png");
	_spHpArray[7] = Sprite::Create(_textureHandle, {826, 478});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp09.png");
	_spHpArray[8] = Sprite::Create(_textureHandle, {818, 489});
}

void GameUI::SpriteHp_Draw() {
	float hp = _playerObj->GetHp();
	float hpMax = _playerObj->GetHpMax();
	if (hp >= hpMax) {
		_spHpArray[0]->Draw();
		_spHpArray[1]->Draw();
		_spHpArray[2]->Draw();
		_spHpArray[3]->Draw();
		_spHpArray[4]->Draw();
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.9f) {
		_spHpArray[1]->Draw();
		_spHpArray[2]->Draw();
		_spHpArray[3]->Draw();
		_spHpArray[4]->Draw();
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.8f) {
		_spHpArray[2]->Draw();
		_spHpArray[3]->Draw();
		_spHpArray[4]->Draw();
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.7f) {
		_spHpArray[3]->Draw();
		_spHpArray[4]->Draw();
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.6f) {
		_spHpArray[4]->Draw();
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.5f) {
		_spHpArray[5]->Draw();
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.4f) {
		_spHpArray[6]->Draw();
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.3f) {
		_spHpArray[7]->Draw();
		_spHpArray[8]->Draw();
	} else if (hp >= hpMax * 0.2f) {
		_spHpArray[8]->Draw();
	}
}

Sprite* GameUI::MyCreate(const std::string& fileName, Vector2 pos) {
	Sprite* sp = nullptr;
	_textureHandle = TextureManager::Load(fileName);
	sp = Sprite::Create(_textureHandle, pos);
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

	// Player的两个瞄准UI
	_spArrow2 = MyCreate("Sprites/arrow.png", {_width / 2, _height / 2});
	_spList.push_back(_spArrow2);
	_spArrow1 = MyCreate("Sprites/arrow_mouse.png", {_width / 2, _height / 2});
	_spList.push_back(_spArrow1);
	// Hp & Speed
	SpriteHp_Initalize();
	_spSpeed = MyCreate("Sprites/speed.png", _posStartSpeed);
	_spSpeed->SetAnchorPoint({1, 0.5f});
	_spList.push_back(_spSpeed);
}

void GameUI::Update() {
	// Player的两个瞄准UI
	_spArrow1->SetPosition(_playerObj->GetArrowMouse());
	_spArrow2->SetPosition(_playerObj->GetArrowMove());
	// Speed
	float realGas = 0; // 因为油门的数值有正有负，所以要特殊处理
	if (_playerObj->GetMoveGasPedal() < 0)
		realGas = (_playerObj->GetMoveGasMax() * 0.5f) - (_playerObj->GetMoveGasPedal() * -1);
	else
		realGas = (_playerObj->GetMoveGasMax() * 0.5f) + _playerObj->GetMoveGasPedal();
	float gasScale = realGas / (_playerObj->GetMoveGasMax() * 1.5f); // 油门的比例
	Vector2 posScale = (_posEndSpeed - _posStartSpeed) * gasScale;
	_spSpeed->SetPosition(_posStartSpeed + posScale);
}

void GameUI::Draw() {
	for (Sprite* it : _spList)
		it->Draw();
	SpriteHp_Draw();
}
