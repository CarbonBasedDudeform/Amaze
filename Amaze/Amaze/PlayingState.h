#pragma once
#include "GameState.h"
#include "GridBlock.h"
#include "Vector.h"

//stl
#include <deque>


class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init() override;
	void Render(sf::RenderWindow * window) override;
	void GenerateMaze(int size);
private:
	const int MAX_ATTEMPTS = 10; //max attempts at generating random values when creating routes

	int _size;
	std::deque<GridBlock *> *  _maze;
	GridLocation * _start;
	GridLocation * _finish;

	void CreateStartAndFinishLocations(GridLocation &, GridLocation &);
	void CreateFauxRoutes(int);
	GridLocation * CreateDeadend(GridLocation &);
	bool MeetsConstraints(GridLocation &, GridLocation &);
	int FindDistance(GridLocation &, GridLocation &);
	void CreateRoute(GridLocation &, GridLocation &);
};

