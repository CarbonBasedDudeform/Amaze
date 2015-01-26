#pragma once
#include "GameState.h"

class MenuState :
	public GameState
{
public:
	MenuState();
	~MenuState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
};

