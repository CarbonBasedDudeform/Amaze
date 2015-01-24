#pragma once
#include "GameState.h"
#include <list>

class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
	std::list<sf::RectangleShape *> * GenerateMaze(int size);
private:
	std::list<sf::RectangleShape *> *  _walls;
};

