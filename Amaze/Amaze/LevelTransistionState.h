#pragma once
#include "GameState.h"
class LevelTransistionState :
	public GameState
{
public:
	LevelTransistionState();
	~LevelTransistionState();
	void Init(int) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
};

