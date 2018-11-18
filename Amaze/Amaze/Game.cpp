#include "Game.h"
#include <cassert>

sf::String Game::GAME_TITLE = "Amaze";

int Game::_count = 0;
Game::Game()
{
	_window = std::make_unique< sf::RenderWindow>(sf::VideoMode(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT), GAME_TITLE, GameProperties::MODE);
	_window->setFramerateLimit(60);
	++_count;
	assert(_count <= 1);
}  

bool Game::Init()
{
	return true;
}

void Game::Loop()
{
	while (_window->isOpen())
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
		}
		auto now = _clock.now();
		std::chrono::duration<float> timeDelta = now - _lastUpdate;
		float delta = timeDelta.count() * 1000;
		_curGameState = _stateManager.GetCurrent();
		//check user input
		_curGameState->ProcessInput(delta);
		//render
		_window->clear();
		_curGameState->Render(_window.get());
		_window->display();
		_curGameState->Update(_stateManager);
		_lastUpdate = now;
	}
}