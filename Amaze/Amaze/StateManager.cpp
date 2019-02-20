#include "StateManager.h"
#include "MenuState.h"


StateManager::StateManager()
	: _size(START_SIZE)
	, score(10)
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
		score = 10;
		_curState = std::make_unique<MenuState>();
		break;
	case Playing:
	{
		score *= score;
		auto options = PlayingStateOptions(++_size, _size/3);
		auto playingSt = std::make_unique<PlayingState>();
		playingSt->Init(options);
		_curState = std::move(playingSt);
		break;
	}
	case GameOver:
	{
		auto gameOverSt = std::make_unique<GameOverState>();
		gameOverSt->SetScore(score);
		_curState = std::move(gameOverSt);
		break;
	}
	default:
		_curState = std::make_unique<MenuState>();
		break;
	}
}
