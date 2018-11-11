#pragma once
#include <SFML/Graphics.hpp>
#include "GridBlock.h"
#include "PhysicsSystem.h"

class Maze
{
public:
	Maze(PhysicsSystem * physics);
	~Maze();
	void GenerateMaze(int size);
	void Render(sf::RenderWindow * window, int bubbleSize, Pawn * hero);
	GridBlock * GetFinish() const;
	GridBlock * GetStart() const;
	int GetSize() const;
	const std::vector<std::unique_ptr<GridLocation>>& GetDeadends() const;
	const GridBlock& GetBlock(int x, int y);
private:
	const int MAX_ATTEMPTS = 50; //max attempts at generating random values when creating routes
	int _size;
	std::unique_ptr<std::vector<std::unique_ptr<GridBlock>>> _maze;
	std::unique_ptr<GridLocation> _start;
	std::unique_ptr<GridLocation> _finish;
	GridBlock * _finishPoint;
	GridBlock * _startPoint;
	PhysicsSystem * _physics;
	std::vector<std::unique_ptr<GridLocation>> _deadends;

	void CreateStartAndFinishLocations(GridLocation &, GridLocation &);
	std::vector<std::unique_ptr<GridLocation>> CreateFauxRoutes(unsigned int);
	void CreateStart(int, int);
	void CreateFinish(int, int);
	std::unique_ptr<GridLocation> CreateDeadend(GridLocation &);
	bool MeetsConstraints(GridLocation &, GridLocation &);
	int FindDistance(GridLocation &, GridLocation &);
	void CreateRoute(GridLocation, GridLocation);
	GridBlock * MultiHack(int, int); //util method for accessing the vector like a 2D array
	void DetectCollidibles(GridBlock &);
	float DistanceToHero(Pawn*, Pawn*);
};

