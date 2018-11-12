#pragma once
#include "GameState.h"
/*
	Class that will manage what is displayed and can be done when a level is being loaded.
*/

class LevelTransistionState :
	public GameState
{
public:
	LevelTransistionState();
	~LevelTransistionState();
	void Init(GameStateOptions opts) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput(float delta) override;
};

