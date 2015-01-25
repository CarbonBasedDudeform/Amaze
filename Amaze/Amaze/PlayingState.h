#pragma once
#include "GameState.h"
#include "GridBlock.h"
#include "Vector.h"
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
	int _size;
	std::deque<GridBlock *> *  _maze;
	void CreateStartAndFinishLocations(GridLocation &, GridLocation &);
	bool MeetsConstraints(GridLocation &, GridLocation &);
	int FindDistance(GridLocation &, GridLocation &);
	void CreateRoute(GridLocation &, GridLocation &, std::deque<GridBlock*>&);
};

