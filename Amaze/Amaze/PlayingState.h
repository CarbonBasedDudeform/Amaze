#pragma once
#include "GameState.h"
class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
};

