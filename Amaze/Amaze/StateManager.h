#pragma once
#include "GameState.h"

class StateManager
{
public:
	StateManager();
	~StateManager();
private:
	int _size;
	std::unique_ptr<GameState> _curState;

};

