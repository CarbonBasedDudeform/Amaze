#pragma once
#include "GameState.h"
class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();
	void Init(GameStateOptions opts) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
};

