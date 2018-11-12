#pragma once
#include "GameState.h"
#include "GameStates.h"

/*
	A fairly dumb class that will manage the scene / state of the game when the menu is being displayed.
*/

class MenuState :
	public GameState
{
public:
	MenuState();
	~MenuState();
	void Init(GameStateOptions opt) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput(float delta) override;
	GameState * Update() override;
private:
	GameState * _nextState;
	static const int INITIAL_MAZE_SIZE = 5;
};

