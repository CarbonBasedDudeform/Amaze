#pragma once
#include <SFML/Graphics.hpp>
#include "GameStates.h"
#include "GameProperties.h"
#include <memory>
#include <chrono>

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

