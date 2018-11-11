#include "PhysicsSystem.h"


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

void PhysicsSystem::RemoveCollidable(Pawn * pawn) {
	
	for (int i = 0; i < _collidables->size(); i++){
		//if collidable[i] points to the same memory location as the pawn they're the same thing
		if (_collidables->at(i) == pawn) {
			_collidables->erase(_collidables->begin() + i);
			return;
		}
	}
}

void PhysicsSystem::Reset()
{
	_collidables->clear();
}

BlockedDirections PhysicsSystem::RayCastCollide(Pawn * from, int distance)
{
	BlockedDirections temp;
	temp.Left.Distance = distance;
	temp.Right.Distance = distance;
	temp.Up.Distance = distance;
	temp.Down.Distance = distance;

	for (int buffer = 0; buffer < distance; buffer += 1)
	{
		for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
		{
			if ((*iter) == from) continue;
			if (temp.Left.Blocked && temp.Right.Blocked && temp.Up.Blocked && temp.Down.Blocked) return std::move(temp);

			if (!temp.Left.Blocked)
			{
				temp.Left.Blocked = AreColliding(from->WorldX - buffer, from->WorldY, from->Size, (*iter));
				if (temp.Left.Blocked) temp.Left.Distance = buffer;
			}
			if (!temp.Right.Blocked)
			{
				temp.Right.Blocked = AreColliding(from->WorldX + buffer, from->WorldY, from->Size, (*iter));
				if (temp.Right.Blocked) temp.Right.Distance = buffer;
			}
			if (!temp.Up.Blocked)
			{
				temp.Up.Blocked = AreColliding(from->WorldX, from->WorldY - buffer, from->Size, (*iter));
				if (temp.Up.Blocked) temp.Up.Distance = buffer;
			}
			if (!temp.Down.Blocked)
			{
				temp.Down.Blocked = AreColliding(from->WorldX, from->WorldY + buffer, from->Size, (*iter));
				if (temp.Down.Blocked) temp.Down.Distance = buffer;
			}
		}
	}

	return std::move(temp);
}

BlockedDirections PhysicsSystem::IsColliding(Pawn * pawn)
{
	int buffer = 5;
	BlockedDirections temp;
	for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
	{
		if ((*iter) == pawn) continue;

		if (!temp.Left.Blocked) temp.Left.Blocked = AreColliding(pawn->WorldX - buffer, pawn->WorldY, pawn->Size, (*iter));
		if (!temp.Right.Blocked) temp.Right.Blocked = AreColliding(pawn->WorldX + buffer, pawn->WorldY, pawn->Size, (*iter));
		if (!temp.Up.Blocked) temp.Up.Blocked = AreColliding(pawn->WorldX, pawn->WorldY - buffer, pawn->Size, (*iter));
		if (!temp.Down.Blocked) temp.Down.Blocked = AreColliding(pawn->WorldX, pawn->WorldY + buffer, pawn->Size, (*iter));
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