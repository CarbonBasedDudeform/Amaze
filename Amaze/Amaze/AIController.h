#pragma once
#include "Controller.h"
#include "AIPawn.h"
#include "PhysicsSystem.h"
#include "GridBlock.h"
#include "Maze.h"
#include "HeroPawn.h"
#include "Laser.h"

#include <memory>
#include <vector>

/*
	Using a BDI architecture. 
	The AI controller's Desire will be to locate and trap the player
	The Belief will be generated based on the desire. It will be the believed location of the player
	The Intention will be to move in such a way as to fulfil the desire of finding and trapping the player.
*/

//The belief holds a location, believed to be the heros location ... or any other location.
struct Belief {
	float WorldX;
	float WorldY;
};

//the intention holds a bool, indicating which direction the AIs intends to travel in
//or an int representing the number of searches it has done in the past inentions.
class Intention {
public:
	Intention() : Left(false), Right(false), Up(false), Down(false), Searching(0), Shoot(false) {};
	Intention(Intention& intent) : Left(intent.Left), Right(intent.Right), Up(intent.Up), Down(intent.Down), Searching(intent.Searching), Shoot(intent.Shoot) {};
	bool Left;
	bool Right;
	bool Up;
	bool Down;
	int Searching;
	bool Shoot;
};

class AIPawnWrapper {
public:
	AIPawnWrapper() {};
	Intention MyIntention;
	AIPawn * pawn;
};

class AIController :
	public Controller
{
public:
	AIController(std::vector<std::unique_ptr<AIPawn>> *, Maze *, PhysicsSystem *,HeroPawn *);
	~AIController();
	void Process(BlockedDirections, float timeDelta) override;
	std::vector<std::unique_ptr<Laser>> Lasers;

private:
	const int VIEW_DISTANCE = 75;
	const int BLOCKING_DISTANCE = 25;
	float accumulatedTime;
	std::unique_ptr<std::vector<std::unique_ptr<AIPawnWrapper>>> _pawns;
	
	void MoveIntoSpace(AIPawnWrapper *, float timeDelta);

	bool MoveLeft(AIPawn *, float timeDelta);
	bool MoveRight(AIPawn *, float timeDelta);
	bool MoveUp(AIPawn *, float timeDelta);
	bool MoveDown(AIPawn *, float timeDelta);

	PhysicsSystem * _physics;
	Maze * _maze;
	Belief _lastLocation;
	HeroPawn * _hero;
	float DistanceToHero(Pawn * pawn, Pawn * hero);
	float DistanceToLocation(Pawn * pawn, Belief& location);
	Belief RandomLocationNear(Belief& lastBelief);
	Intention DecideIntent(AIPawnWrapper * pawn, Intention);
	Intention Explore(const BlockedDirections& blocked, Intention& previousIntent) const;
	Intention Investigate(Pawn * pawn, const Intention& previousIntent);
	Intention ShootToKill();
};

