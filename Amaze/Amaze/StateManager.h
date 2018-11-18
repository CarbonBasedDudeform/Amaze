#pragma once
#include "GameState.h"

class StateManager
{
public:
	enum States { Menu, Playing, NextLevel, GameOver};
	StateManager();
	~StateManager();
	GameState * GetCurrent() const;
	void ChangeState(States state);
private:
	int _size;
	std::unique_ptr<GameState> _curState;

};

