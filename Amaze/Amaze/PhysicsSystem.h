#pragma once
#include "Pawn.h"
#include <vector>

/*
	Direction contains the distance to the collidable that is blocking
	if nothing is blocking then it is the max check size which depends on the input to the physics system.
	Also contains a bool to indicate if something was hit, as a quick check for if something was blocked or not.
*/
struct Direction
{
	Direction() : Blocked(false), Distance(0) {}
	Direction(const Direction &copy) : Blocked(copy.Blocked), Distance(copy.Distance) {}
	bool Blocked;
	int Distance;
};

/*
	BlockedDirections is a helper for containing the left right up and down directions and checking if theyre blocked.
*/
struct BlockedDirections
{
	BlockedDirections() :Left(Direction()), Right(Direction()), Up(Direction()), Down(Direction()) {}
	BlockedDirections(const BlockedDirections &copy) : Left(copy.Left), Right(copy.Right), Up(copy.Up), Down(copy.Down) {}
	Direction Left;
	Direction Right;
	Direction Up;
	Direction Down;
};

/*
	A poor mans physics system,
	Does some rough collision detection
	no Runge-Kutta here, just a weird choice of circular collision detection for a game that is mostly squares.
*/

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

