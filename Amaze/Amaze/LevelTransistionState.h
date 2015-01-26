#pragma once
#include "GameState.h"
class LevelTransistionState :
	public GameState
{
public:
	LevelTransistionState();
	~LevelTransistionState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
};

