#pragma once
#include "Controller.h"
#include "AIPawn.h"
#include "PhysicsSystem.h"
#include "GridBlock.h"

#include <memory>
#include <vector>

/*
	Using a BDI architecture. 
	The AI controller's Desire will be to locate and trap the player
	The Belief will be generated based on the desire. It will be the believed location of the player
	The Intention will be to move in such a way as to fulfil the desire of finding and trapping the player.
*/

//The belief holds a location, believed to be the heros location
class Belief {
public:
	Belief() : X(0), Y(0), WorldX(0), WorldY(0) {};
	~Belief() {};
	int X;
	int Y;
	float WorldX;
	float WorldY;
};

//the intention holds a bool, indicating which direction the AIs intends to travel in
class Intention {
public:
	Intention() : Left(false), Right(false), Up(false), Down(false) {};
	Intention(Intention& intent) : Left(intent.Left), Right(intent.Right), Up(intent.Up), Down(intent.Down) {};
	~Intention() {};
	bool Left;
	bool Right;
	bool Up;
	bool Down;
};

class AIPawnWrapper {
public:
	AIPawnWrapper() {};
	~AIPawnWrapper() {};
	Intention MyIntention;
	bool PreviousMoveOK;
	AIPawn * pawn;
};

class AIController :
	public Controller
{
public:
	AIController(std::vector<std::unique_ptr<AIPawn>> *, int);
	~AIController();
	void Process(BlockedDirections, float timeDelta) override;

private:
	std::unique_ptr<std::vector<std::unique_ptr<AIPawnWrapper>>> _pawns;
	
	void FindOpenSpace();
	void MoveIntoSpace(AIPawnWrapper *);

	bool MoveLeft(AIPawn *);
	bool MoveRight(AIPawn *);
	bool MoveUp(AIPawn *);
	bool MoveDown(AIPawn *);

	std::unique_ptr<PhysicsSystem> _physics;

	std::unique_ptr<Belief> _heroLocation;
	Intention DecideIntent(Intention);
};

