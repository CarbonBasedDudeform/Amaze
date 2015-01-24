#include "Game.h"

sf::String Game::GAME_TITLE = "Amaze";

Game * Game::_instance = nullptr;
Game * Game::GetInstance()
{
	//Not safe if multithreading every gets implemented
	if (_instance) return _instance;
	else _instance = new Game();

	return _instance;
}

Game::Game()
{
}


Game::~Game()
{
}

bool Game::Init()
{
	_window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE);
	_curGameState = new PlayingState();
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

		_window->clear();
		_curGameState->Render(_window);
		_window->display();
	}
}