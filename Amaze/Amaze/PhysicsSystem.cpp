#include "PhysicsSystem.h"


PhysicsSystem * PhysicsSystem::_instance = nullptr;
PhysicsSystem * PhysicsSystem::GetInstance() {
	if (_instance == nullptr) _instance = new PhysicsSystem();

	return _instance;
}

PhysicsSystem::PhysicsSystem()
{
	_collidables = new std::vector<Pawn *>();
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::AddCollidable(Pawn * pawn)
{
	//update this to a bet container
	for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
	{
		if (*iter == pawn) return;
	}
	_collidables->push_back(pawn);
}

#include <iostream>
void PhysicsSystem::CleanUp(Pawn * pawn) {
	for (int i = 0; i < _collidables->size(); i++){
		if (&(*(_collidables->at(i))) == &(*pawn)) {
			_collidables->erase(_collidables->begin() + i);
			return;
		}
	}
}

BlockedDirections PhysicsSystem::IsColliding(Pawn * pawn)
{
	int buffer = 1;
	BlockedDirections temp;
	for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
	{
		if (!temp.Left) {
			temp.Left = AreColliding(pawn->WorldX - buffer, pawn->WorldY, pawn->Size, (*iter));

			if (temp.Left) {
				std::cout << "the fuck is this shit" << std::endl; 
			}
		}
		if (!temp.Right) temp.Right = AreColliding(pawn->WorldX + buffer, pawn->WorldY, pawn->Size, (*iter));
		if (!temp.Up) temp.Up = AreColliding(pawn->WorldX, pawn->WorldY - buffer, pawn->Size, (*iter));
		if (!temp.Down) temp.Down = AreColliding(pawn->WorldX, pawn->WorldY + buffer, pawn->Size, (*iter));
	}

	return temp;
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

bool PhysicsSystem::AreColliding(float x1, float y1, int size, Pawn * pwn) {
	// [1]--[2] <-- these are colliding when the distance between them is less than their combined sizes
	float CollisionDistance = ((size + (pwn->Size))) / 2;

	if ((FindDistance(x1, y1, pwn) -  CollisionDistance) < 0.1f)
	{
			FindDistance(x1, y1, pwn);
		return true;
	}

	return false;
}

float PhysicsSystem::FindDistance(float x1, float y1, Pawn * pwn) {
	float xDiff = pwn->WorldX - x1;
	float yDiff = pwn->WorldY - y1;

	return sqrt((yDiff*yDiff) + (xDiff*xDiff));
}