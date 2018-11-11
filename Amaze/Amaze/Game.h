#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"
#include "GameProperties.h"
#include <memory>

class Game
{
public:
	Game();
	static sf::String GAME_TITLE;

	bool Init();
	void Loop();
private:
	static int _count;
	GameState * _curGameState;
	std::unique_ptr<sf::RenderWindow> _window;

};

