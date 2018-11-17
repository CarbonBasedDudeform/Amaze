#include "StateManager.h"
#include "MenuState.h"


StateManager::StateManager()
	: _size(10)
{
	_curState = std::make_unique<MenuState>();
}


StateManager::~StateManager()
{
}
