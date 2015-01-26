#pragma once
#include "GameState.h"
#include "GridBlock.h"
#include "HeroController.h"
#include "HeroPawn.h"
#include "PhysicsSystem.h"

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
	void ProcessInput() override;
	void GenerateMaze(int size);

private:
	const int MAX_ATTEMPTS = 10; //max attempts at generating random values when creating routes

	//THE MAZE
	int _size;
	std::deque<GridBlock *> *  _maze;
	GridLocation * _start;
	GridLocation * _finish;

	void CreateStartAndFinishLocations(GridLocation &, GridLocation &);
	void CreateFauxRoutes(unsigned int);
	GridLocation * CreateDeadend(GridLocation &);
	bool MeetsConstraints(GridLocation &, GridLocation &);
	int FindDistance(GridLocation &, GridLocation &);
	void CreateRoute(GridLocation &, GridLocation &);

	void CreateStart(int, int);
	void CreateFinish(int, int);

	PhysicsSystem * _physics;

	//The Hero (Thero)
	HeroPawn * _hero;
	HeroController * _heroController;

};

