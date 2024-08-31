#pragma once
#include "Input.h"
#include "Player.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "cmath"
#include "vector"

class GameUI {
public:
	uint32_t _textureHandle = 0;
	float _width = 0;
	float _height = 0;
	Player* _playerObj = nullptr;

	std::vector<Sprite*> _spList{}; // 用来储存所有声明出来的对象，方便统一调用和销毁
	Sprite* _spArrow1 = nullptr;
	Sprite* _spArrow2 = nullptr;
	Sprite* _spHps[9];
	Sprite* _spSpeed = nullptr;
	Vector2 _posStartSpeed = {452, 494};
	Vector2 _posEndSpeed = {409, 404};

	int _score = 0;
	int _huretScore = 10;
	int _deadScore = 100;
	Vector2 _scoreSize = {18, 30};
	float _scoreSizeScale = 1.f;
	float _scoreNextLength = 2 + _scoreSize.x * _scoreSizeScale; // 从贴图左上角开始，下一个贴图的左上角的距离
	Sprite* _spNumbers[10];
	uint32_t _textureNumbers[10];
	Vector2 _posScore{};
	std::vector<int> _scoreDigits; // 储存着分数的每一位数字

	void Hp_Initalize();
	void Hp_Draw();
	void Numbers_Initalize();
	void Score_Draw();

	Sprite* MyCreate(const std::string& fileName, Vector2 pos);

public:
	~GameUI();
	void Initalize(int screenWidth, int screenHeight, Player* playerObj);
	void Update();
	void Draw();

	void HurtScore() { _score += _huretScore; };
	void DeadScore() { _score += _deadScore; };
};
