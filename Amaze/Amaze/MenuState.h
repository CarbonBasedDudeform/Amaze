#pragma once
#include "GameState.h"
#include "GameStates.h"

class MenuState :
	public GameState
{
public:
	MenuState();
	~MenuState();
	void Init(GameStateOptions opt) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
	GameState * Update() override;
private:
	GameState * _nextState;
	static const int INITIAL_MAZE_SIZE = 5;
};

