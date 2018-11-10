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
	//update this to a better container
	for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
	{
		if (*iter == pawn) return;
	}
	_collidables->push_back(pawn);
}

void PhysicsSystem::CleanUp(Pawn * pawn) {
	
	for (int i = 0; i < _collidables->size(); i++){
		//if collidable[i] points to the same memory location as the pawn they're the same thing
		if (&(*(_collidables->at(i))) == &(*pawn)) {
			_collidables->erase(_collidables->begin() + i);
			return;
		}
	}

	//_collidables->clear();
}

void PhysicsSystem::Reset()
{
	_collidables->clear();
}

BlockedDirections PhysicsSystem::IsColliding(Pawn * pawn)
{
	int buffer = 5;
	BlockedDirections temp;
	for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
	{
		if (!temp.Left) temp.Left = AreColliding(pawn->WorldX - buffer, pawn->WorldY, pawn->Size, (*iter));
		if (!temp.Right) temp.Right = AreColliding(pawn->WorldX + buffer, pawn->WorldY, pawn->Size, (*iter));
		if (!temp.Up) temp.Up = AreColliding(pawn->WorldX, pawn->WorldY - buffer, pawn->Size, (*iter));
		if (!temp.Down) temp.Down = AreColliding(pawn->WorldX, pawn->WorldY + buffer, pawn->Size, (*iter));
	}

	return temp;
}

bool PhysicsSystem::AreColliding(float x1, float y1, int size, Pawn * pwn) {
	// [1]--[2] <-- these are colliding when the distance between them is less than their combined sizes
	float CollisionDistance = ((size + (pwn->Size)) / 2);

	if ((FindDistance(x1, y1, pwn) -  CollisionDistance) < 0.1f)
	{
			return true;
	}

	return false;
}

bool PhysicsSystem::AreColliding(Pawn * one, Pawn * two)
{
	return AreColliding(one->WorldX, one->WorldY, one->Size, two);
}

float PhysicsSystem::FindDistance(float x1, float y1, Pawn * pwn) {
	float xDiff = pwn->WorldX - x1;
	float yDiff = pwn->WorldY - y1;

	return sqrt((yDiff*yDiff) + (xDiff*xDiff));
}