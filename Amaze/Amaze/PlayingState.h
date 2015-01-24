#pragma once
#include "GameState.h"
#include "GridBlock.h"
#include <deque>

class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
	std::deque<GridBlock *> * GenerateMaze(int size);
private:
	std::deque<GridBlock *> *  _maze;
	void CreateStartAndFinishLocations(GridLocation &, GridLocation &, int mazeSize);
	int FindDistance(GridLocation &, GridLocation &);
};

