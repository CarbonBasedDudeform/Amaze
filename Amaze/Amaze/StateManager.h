#pragma once
#include "GameState.h"

class StateManager
{
public:
	static const int START_SIZE = 10;
	enum States { Menu, Playing, NextLevel, GameOver};
	StateManager();
	~StateManager();
	GameState * GetCurrent() const;
	void ChangeState(States state);
private:
	int _size;
	int score;
	std::unique_ptr<GameState> _curState;

};

