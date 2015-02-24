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
	_window = new sf::RenderWindow(sf::VideoMode(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT), GAME_TITLE);
	_curGameState = new PlayingState();
}


Game::~Game()
{
	delete _window;
	delete _curGameState;
}

bool Game::Init()
{
	_curGameState->Init(5);
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
		_curGameState = _curGameState->Update();

		//check user input
		_curGameState->ProcessInput();
		//render
		_window->clear();
		_curGameState->Render(_window);
		_window->display();
	}
}