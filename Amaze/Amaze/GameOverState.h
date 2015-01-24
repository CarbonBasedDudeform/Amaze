#pragma once
#include "GameState.h"
class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
};

