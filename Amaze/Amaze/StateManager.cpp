#include "StateManager.h"
#include "MenuState.h"


StateManager::StateManager()
	: _size(START_SIZE)
{
	_curState = std::make_unique<MenuState>();
}


StateManager::~StateManager()
{
}

GameState * StateManager::GetCurrent() const
{
	return _curState.get();
}

void StateManager::ChangeState(States state)
{
	switch (state)
	{
	case Menu:
		_size = START_SIZE;
		_curState = std::make_unique<MenuState>();
		break;
	case Playing:
	{
		auto options = PlayingStateOptions(++_size, _size);
		auto playingSt = std::make_unique<PlayingState>();
		playingSt->Init(options);
		_curState = std::move(playingSt);
		break;
	}
	case GameOver:
	{
		auto gameOverSt = std::make_unique<GameOverState>();
		gameOverSt->SetScore(_size * 100);
		_curState = std::move(gameOverSt);
		break;
	}
	default:
		_curState = std::make_unique<MenuState>();
		break;
	}
}
