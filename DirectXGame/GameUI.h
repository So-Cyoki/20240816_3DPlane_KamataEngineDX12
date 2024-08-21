#pragma once
#include "Input.h"
#include "Player.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "cmath"
#include "vector"

class GameUI {
private:
	uint32_t _textureHandle = 0;
	float _width = 0;
	float _height = 0;
	Player* _playerObj = nullptr;

	std::vector<Sprite*> _spList{}; // 用来储存所有声明出来的对象，方便统一调用和销毁
	Sprite* _spArrow1 = nullptr;
	Vector2 _spArrow1Pos{};
	Sprite* _spArrow2 = nullptr;

	Sprite* MyCreate(const std::string& fileName);

public:
	~GameUI();
	void Initalize(int screenWidth, int screenHeight, Player* playerObj);
	void Update();
	void Draw();
};
