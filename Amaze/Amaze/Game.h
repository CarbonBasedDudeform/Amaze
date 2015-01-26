#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"
#include "GameProperties.h"

class Game
{
public:
	~Game();
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

