#pragma once
#include "GameState.h"

/*
	Class that will manage the game over scene.
*/

class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();
	void Init(GameStateOptions opts) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput(float delta) override;
};

