#pragma once
#include "Pawn.h"
#include <vector>

struct Direction
{
	Direction() : Blocked(false), Distance(0) {}
	Direction(const Direction &copy) : Blocked(copy.Blocked), Distance(copy.Distance) {}
	bool Blocked;
	int Distance;
};

struct BlockedDirections
{
	BlockedDirections() :Left(Direction()), Right(Direction()), Up(Direction()), Down(Direction()) {}
	BlockedDirections(const BlockedDirections &copy) : Left(copy.Left), Right(copy.Right), Up(copy.Up), Down(copy.Down) {}
	Direction Left;
	Direction Right;
	Direction Up;
	Direction Down;
};

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	
	BlockedDirections IsColliding(Pawn*);

	bool AreColliding(float, float, int, Pawn*);
	bool AreColliding(Pawn*, Pawn*);
	float FindDistance(float, float, Pawn*);

	void AddCollidable(Pawn *);
	void RemoveCollidable(Pawn *);
	void Reset();
	BlockedDirections RayCastCollide(Pawn * from, int distance);
private:


	std::vector<Pawn *> * _collidables;
};

