#pragma once
#include <SFML\Graphics\Shader.hpp>
#include "GameState.h"
#include "GridBlock.h"

#include "HeroController.h"
#include "HeroPawn.h"

#include "AIController.h"
#include "AIPawn.h"

#include "PhysicsSystem.h"

//stl
#include <vector>

struct PlayingStateOptions {
	int LevelSize;
	PlayingStateOptions(int size) : LevelSize(size) {};
};

class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init(PlayingStateOptions opts);
	void Render(sf::RenderWindow * window) override;
	void ProcessInput() override;
	GameState * Update() override;
	void GenerateMaze(int size);

private:
	const int MAX_ATTEMPTS = 50; //max attempts at generating random values when creating routes
	GridBlock * MultiHack(int, int); //util method for accessing the vector like a 2D array
	//THE MAZE
	int _size;
	std::vector<GridBlock *> *  _maze;
	GridLocation * _start;
	GridLocation * _finish;
	GridBlock * _finishPoint;

	void CreateStartAndFinishLocations(GridLocation &, GridLocation &);
	void CreateFauxRoutes(unsigned int);
	GridLocation * CreateDeadend(GridLocation &);
	bool MeetsConstraints(GridLocation &, GridLocation &);
	int FindDistance(GridLocation &, GridLocation &);
	void CreateRoute(GridLocation, GridLocation);
	void DetectCollidibles(GridBlock &);
	void CleanUp();

	void CreateStart(int, int);
	void CreateFinish(int, int);

	PhysicsSystem * _physics;

	//The Hero (Thero)
	HeroPawn * _hero;
	HeroController * _heroController;

	//The Terrors (The T errors)
	std::vector<AIPawn *> * _terrors;
	AIController * _terrorsController;
	int _placedAI;

	static const float BUBBLE_SIZE;
	float DistanceToHero(Pawn*);

};

