#pragma once
#include "Input.h"
#include "Player.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "cmath"
#include "vector"

class UIManager {

	// 没有时间搞那么复杂了，直接在这里通过枚举控制好了
public:
	enum class Scene { Start, Loading, Game, Dead };
	inline static Scene _currentScene = Scene::Start;
};

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
	Sprite* _spHelp = nullptr;
	Vector4 _colorHelp = {1, 1, 1, 1};

	Sprite* _spkillPoint = nullptr;
	float _colorKillPoint_speed = 0.05f; // 减少Alpha值的速度
	float _scaleKillPoint_speed = 0.2f;  // 调整大小的速度
	float _scaleKillPoint_max = 2.5f;
	float _scaleKillPoint = 1;
	Vector2 _killPointSize = {22, 22};
	Vector4 _colorKillPoint = {1, 1, 1, 0};

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

	Sprite* MyCreate(const std::string& fileName, Vector2 pos);

public:
	~GameUI();
	void Initalize(int screenWidth, int screenHeight, Player* playerObj);
	void Update();
	void Draw();

	void Score_Draw(Vector2 startPos);

	void HurtScore() { _score += _huretScore; };
	void DeadScore() { _score += _deadScore; };
	void AniStart_skillPoint() {
		_colorKillPoint = {1, 1, 1, 1};
		_scaleKillPoint = 1;
	};
	void AniStart_help() {
		if (_colorHelp.w > 0) {
			_colorHelp.w -= 0.01f;
			if (_colorHelp.w < 0)
				_colorHelp.w = 0;
		}
	}

	const int& GetScore() const { return _score; };
	void SetScore(const int& score) { _score = score; };
	void SetColor_killPoint(const Vector4& color) { _colorKillPoint = color; };
	const Vector4& GetColorHelp() const { return _colorHelp; };
	void SetColorHelp(const Vector4& color) { _colorHelp = color; };
};

class DeadUI {
private:
	uint32_t _textureHandle = 0;
	float _width = 0;
	float _height = 0;
	Sprite* _spDeadTitle = nullptr;
	Vector2 _posDeadTitle = {0, 0};
	Sprite* _spRestart = nullptr;
	Vector2 _posRestart = {381, 568};

	bool _isRestart = false; // 下一个场景

public:
	~DeadUI();
	void Initalize(int screenWidth, int screenHeight);
	void Update();
	void Draw();

	const bool& GetIsRestart() const { return _isRestart; };
	void SetIsRestart(const bool& flag) { _isRestart = flag; };
};

class StartUI {
private:
	uint32_t _textureHandle = 0;
	float _width = 0;
	float _height = 0;
	Sprite* _spStartTitle = nullptr;
	Vector2 _posStartTitle = {0, 0};
	Vector4 _colorStartTitle = {1, 1, 1, 1};
	Sprite* _spStart = nullptr;
	Vector2 _posStart = {399, 589};

	bool _isStart = false; // 下一个场景

public:
	~StartUI();
	void Initalize(int screenWidth, int screenHeight);
	void Update();
	void Draw();

	const bool& GetIsStart() const { return _isStart; };
	void SetIsStart(const bool& flag) { _isStart = flag; };
};