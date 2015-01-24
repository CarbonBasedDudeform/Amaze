#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"

class Game
{
public:
	~Game();
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;
	static sf::String GAME_TITLE;

	static Game * GetInstance();
	bool Init();
	void Loop();
private:
	Game();
	static Game * _instance;
	GameState * _curGameState;
	sf::RenderWindow * _window;

};

