#include "GameUI.h"

void GameUI::Hp_Initalize() {
	_textureHandle = TextureManager::Load("Sprites/Hp/hp01.png");
	_spHps[0] = Sprite::Create(_textureHandle, {861, 399});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp02.png");
	_spHps[1] = Sprite::Create(_textureHandle, {857, 412});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp03.png");
	_spHps[2] = Sprite::Create(_textureHandle, {854, 423});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp04.png");
	_spHps[3] = Sprite::Create(_textureHandle, {849, 434});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp05.png");
	_spHps[4] = Sprite::Create(_textureHandle, {845, 445});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp06.png");
	_spHps[5] = Sprite::Create(_textureHandle, {839, 456});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp07.png");
	_spHps[6] = Sprite::Create(_textureHandle, {833, 467});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp08.png");
	_spHps[7] = Sprite::Create(_textureHandle, {826, 478});
	_textureHandle = TextureManager::Load("Sprites/Hp/hp09.png");
	_spHps[8] = Sprite::Create(_textureHandle, {818, 489});
}

void GameUI::Hp_Draw() {
	float hp = _playerObj->GetHp();
	float hpMax = _playerObj->GetHpMax();
	if (hp >= hpMax) {
		_spHps[0]->Draw();
		_spHps[1]->Draw();
		_spHps[2]->Draw();
		_spHps[3]->Draw();
		_spHps[4]->Draw();
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.9f) {
		_spHps[1]->Draw();
		_spHps[2]->Draw();
		_spHps[3]->Draw();
		_spHps[4]->Draw();
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.8f) {
		_spHps[2]->Draw();
		_spHps[3]->Draw();
		_spHps[4]->Draw();
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.7f) {
		_spHps[3]->Draw();
		_spHps[4]->Draw();
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.6f) {
		_spHps[4]->Draw();
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.5f) {
		_spHps[5]->Draw();
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.4f) {
		_spHps[6]->Draw();
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp >= hpMax * 0.3f) {
		_spHps[7]->Draw();
		_spHps[8]->Draw();
	} else if (hp > 0) {
		_spHps[8]->Draw();
	}
}

void GameUI::Numbers_Initalize() {
	_textureNumbers[0] = TextureManager::Load("Sprites/Numbers/00.png");
	_textureNumbers[1] = TextureManager::Load("Sprites/Numbers/01.png");
	_textureNumbers[2] = TextureManager::Load("Sprites/Numbers/02.png");
	_textureNumbers[3] = TextureManager::Load("Sprites/Numbers/03.png");
	_textureNumbers[4] = TextureManager::Load("Sprites/Numbers/04.png");
	_textureNumbers[5] = TextureManager::Load("Sprites/Numbers/05.png");
	_textureNumbers[6] = TextureManager::Load("Sprites/Numbers/06.png");
	_textureNumbers[7] = TextureManager::Load("Sprites/Numbers/07.png");
	_textureNumbers[8] = TextureManager::Load("Sprites/Numbers/08.png");
	_textureNumbers[9] = TextureManager::Load("Sprites/Numbers/09.png");
	_spNumbers[0] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[1] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[2] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[3] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[4] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[5] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[6] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[7] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[8] = Sprite::Create(_textureNumbers[0], {-100, -100});
	_spNumbers[9] = Sprite::Create(_textureNumbers[0], {-100, -100});
	Vector2 size = _scoreSize * _scoreSizeScale;
	_spNumbers[0]->SetSize(size);
	_spNumbers[1]->SetSize(size);
	_spNumbers[2]->SetSize(size);
	_spNumbers[3]->SetSize(size);
	_spNumbers[4]->SetSize(size);
	_spNumbers[5]->SetSize(size);
	_spNumbers[6]->SetSize(size);
	_spNumbers[7]->SetSize(size);
	_spNumbers[8]->SetSize(size);
	_spNumbers[9]->SetSize(size);
}

void GameUI::Score_Draw() {
	// 将每一位数字都拆开并存放
	_scoreDigits.clear();
	int temp = _score;
	while (temp > 0) {
		int digit = temp % 10;
		_scoreDigits.push_back(digit);
		temp /= 10;
	}
	std::reverse(_scoreDigits.begin(), _scoreDigits.end()); // 翻转要素，让它正确显示数字顺序

	// 画图
	if (_scoreDigits.size() == 0) {
		_spNumbers[0]->SetTextureHandle(_textureNumbers[0]);
		_spNumbers[0]->SetPosition({_posScore.x - _scoreSize.x * _scoreSizeScale * 0.5f, _posScore.y});
		_spNumbers[0]->Draw();
	} else {
		int numDigit = 0;   // 当前显示到第几位
		bool isMax = false; // 是否超过所有可以显示的位数了
		for (int digit : _scoreDigits) {
			_spNumbers[numDigit]->SetTextureHandle(_textureNumbers[digit]);
			Vector2 startPos = {_posScore.x - (_scoreDigits.size() * 0.5f * _scoreNextLength), _posScore.y};
			_spNumbers[numDigit]->SetPosition({startPos.x + _scoreNextLength * numDigit, startPos.y});
			_spNumbers[numDigit]->Draw();
			numDigit++;
			if (numDigit >= 9) {
				isMax = true;
				break;
			}
		}
		if (isMax) {
			for (int i = 0; i < 10; i++) {
				_spNumbers[i]->SetTextureHandle(_textureNumbers[9]);
				Vector2 startPos = {_posScore.x - (10 * 0.5f * _scoreNextLength), _posScore.y};
				_spNumbers[i]->SetPosition({startPos.x + _scoreNextLength * i, startPos.y});
				_spNumbers[i]->Draw();
			}
		}
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
	Hp_Initalize();
	_spSpeed = MyCreate("Sprites/speed.png", _posStartSpeed);
	_spSpeed->SetAnchorPoint({1, 0.5f});
	_spList.push_back(_spSpeed);
	// Score
	Numbers_Initalize();
	_score = 0;
	_posScore = {_width * 0.5f, 27};
	// killPoint
	_spkillPoint = MyCreate("Sprites/killPoint.png", {_width / 2, _height / 2});
	_spList.push_back(_spkillPoint);
	// Help
	_spHelp = MyCreate("Sprites/help.png", {_width / 2, _height / 2});
	_spList.push_back(_spHelp);
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
	// KillPoint
	if (_colorKillPoint.w > 0) {
		_colorKillPoint.w -= _colorKillPoint_speed;
		if (_colorKillPoint.w <= 0)
			_colorKillPoint.w = 0;
	}
	if (_scaleKillPoint < _scaleKillPoint_max) {
		_scaleKillPoint += _scaleKillPoint_speed;
	}
	_spkillPoint->SetColor(_colorKillPoint);
	_spkillPoint->SetSize({_killPointSize.x * _scaleKillPoint, _killPointSize.y * _scaleKillPoint});
	// Help
	_spHelp->SetColor(_colorHelp);
}

void GameUI::Draw() {
	for (Sprite* it : _spList)
		it->Draw();
	Hp_Draw();
	Score_Draw();
}

DeadUI::~DeadUI() {
	delete _spDeadTitle;
	delete _spRestart;
}

void DeadUI::Initalize(int screenWidth, int screenHeight) {
	_width = float(screenWidth), _height = float(screenHeight);

	_textureHandle = TextureManager::Load("Sprites/deadUI.png");
	_spDeadTitle = Sprite::Create(_textureHandle, _posDeadTitle);
	_textureHandle = TextureManager::Load("Sprites/restart.png");
	_spRestart = Sprite::Create(_textureHandle, _posRestart);

	_isRestart = false;
}

void DeadUI::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE))
		_isRestart = true;

	_spRestart->SetPosition(_posRestart);
}

void DeadUI::Draw() {
	_spDeadTitle->Draw();
	_spRestart->Draw();
}

StartUI::~StartUI() {
	delete _spStart;
	delete _spStartTitle;
}

void StartUI::Initalize(int screenWidth, int screenHeight) {
	_width = float(screenWidth), _height = float(screenHeight);

	_textureHandle = TextureManager::Load("Sprites/startUI.png");
	_spStartTitle = Sprite::Create(_textureHandle, _posStartTitle);
	_textureHandle = TextureManager::Load("Sprites/start.png");
	_spStart = Sprite::Create(_textureHandle, _posStart);

	_isStart = false;
}

void StartUI::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE))
		_isStart = true;

	if (_colorStartTitle.w > 0 && _isStart)
		_colorStartTitle.w -= 0.01f;

	_spStartTitle->SetColor(_colorStartTitle);
	_spStart->SetPosition(_posStart);
}

void StartUI::Draw() {
	_spStartTitle->Draw();
	_spStart->Draw();
}
