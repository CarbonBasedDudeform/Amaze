#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"
#include "GameProperties.h"
#include <memory>
#include <chrono>

/*
This class basically holds the game loop.
It creates the window, keeps track of time between frames
and does the classic Update-Input-Render loop.

It's intended use is to be created, initialised then looped.
*/

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
	std::chrono::high_resolution_clock _clock;
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastUpdate;

};

