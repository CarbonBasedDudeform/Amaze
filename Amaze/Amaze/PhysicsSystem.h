#pragma once
#include "Pawn.h"
#include <vector>

class BlockedDirections
{
public:
	BlockedDirections() :Left(false), Right(false), Up(false), Down(false) {}
	BlockedDirections(const BlockedDirections &copy) : Left(copy.Left), Right(copy.Right), Up(copy.Up), Down(copy.Down) {}
	bool Left;
	bool Right;
	bool Up;
	bool Down;
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
	void CleanUp(Pawn *);
	void Reset();
private:


	std::vector<Pawn *> * _collidables;
};

