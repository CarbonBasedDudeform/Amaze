#include "PhysicsSystem.h"


PhysicsSystem * PhysicsSystem::_instance = nullptr;
PhysicsSystem * PhysicsSystem::GetInstance() {
	if (_instance == nullptr) _instance = new PhysicsSystem();

	return _instance;
}

PhysicsSystem::PhysicsSystem()
{
}


PhysicsSystem::~PhysicsSystem()
{
}

bool PhysicsSystem::AreColliding(Pawn * one, Pawn * two)
{
	// [1]--[2] <-- these are colliding when the distance between them is less than their combined sizes
#ifdef NDEBUG
	int CollisionDistance = ((one->Size + (two->Size))) / 2;
#endif
	//debug has a slightly different collision detection sum as the size value for the hero pawn is the radius of the circle (in debug) 
	//and the 'diameter', or width, of the square in release
#ifdef _DEBUG
	int CollisionDistance = ((one->Size + (two->Size)/2));
#endif

	if (FindDistance(one, two) <  CollisionDistance)
	{
		return true;
	}

	return false;
}

float PhysicsSystem::FindDistance(Pawn* one, Pawn * two) 
{
	float xDiff = abs(two->WorldX - one->WorldX);
	float yDiff = abs(two->WorldY - one->WorldY);

	return sqrt(abs((yDiff*yDiff) + (xDiff*xDiff)));
}
