#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem()
{
	_collidables = std::make_unique<std::vector<Pawn *>>();
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::AddCollidable(Pawn * pawn)
{
	if (pawn == nullptr) return;

	const bool IsAlreadyAdded = std::find(_collidables->begin(), _collidables->end(), pawn) != _collidables->end();
	if (IsAlreadyAdded)
	{
		return;
	}

	_collidables->push_back(pawn);
}

void PhysicsSystem::RemoveCollidable(Pawn * pawn) 
{
	auto res = std::find(_collidables->begin(), _collidables->end(), pawn);
	if (res != _collidables->end())
	{
		_collidables->erase(res);
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
	auto worldX = from->WorldX;
	auto worldY = from->WorldY;
	auto Size = from->Size;

	for (int buffer = 0; buffer < distance; buffer += 1)
	{
		for (auto iter = _collidables->begin(); iter != _collidables->end(); iter++)
		{
			if ((*iter) == from) continue;
			if ((*iter)->IsBlocking == false) continue;

			if (temp.Left.Blocked && temp.Right.Blocked && temp.Up.Blocked && temp.Down.Blocked) return std::move(temp);

			if (!temp.Left.Blocked)
			{
				temp.Left.Blocked = AreColliding(worldX - buffer, worldY, Size, (*iter));
				if (temp.Left.Blocked) temp.Left.Distance = buffer;
			}
			if (!temp.Right.Blocked)
			{
				temp.Right.Blocked = AreColliding(worldX + buffer, worldY, Size, (*iter));
				if (temp.Right.Blocked) temp.Right.Distance = buffer;
			}
			if (!temp.Up.Blocked)
			{
				temp.Up.Blocked = AreColliding(worldX, worldY - buffer, Size, (*iter));
				if (temp.Up.Blocked) temp.Up.Distance = buffer;
			}
			if (!temp.Down.Blocked)
			{
				temp.Down.Blocked = AreColliding(worldX, worldY + buffer, Size, (*iter));
				if (temp.Down.Blocked) temp.Down.Distance = buffer;
			}
		}
	}

	return std::move(temp);
}

BlockedDirections PhysicsSystem::IsColliding(Pawn * pawn)
{
	static const int buffer = 5;
	BlockedDirections temp;
	auto worldX = pawn->WorldX;
	auto worldY = pawn->WorldY;
	auto Size = pawn->Size;
	for (auto iter = _collidables->begin(); iter != _collidables->end(); )
	{
		if ((*iter) == pawn) {
			iter++;
			continue;
		}
		
		if ((*iter)->DoesDamage && pawn->TakesDamage && (*iter)->parent != pawn  && AreColliding(worldX, worldY, Size, (*iter))) {
			pawn->DoDamage((*iter)->DamageAmount);
			iter = _collidables->erase(iter);
			continue;
		}

		if ((*iter)->IsBlocking == false) {
			iter++;
			continue;
		}
		
		if (!temp.Left.Blocked) temp.Left.Blocked = AreColliding(worldX - buffer, worldY, Size, (*iter));
		if (!temp.Right.Blocked) temp.Right.Blocked = AreColliding(worldX + buffer, worldY, Size, (*iter));
		if (!temp.Up.Blocked) temp.Up.Blocked = AreColliding(worldX, worldY - buffer, Size, (*iter));
		if (!temp.Down.Blocked) temp.Down.Blocked = AreColliding(worldX, worldY + buffer, Size, (*iter));
		iter++;
	}

	return std::move(temp);
}

bool PhysicsSystem::AreColliding(float x1, float y1, float size, Pawn * pwn) {
	auto worldX = pwn->WorldX;
	auto worldY = pwn->WorldY;
	auto Size = pwn->Size;
	return (
		abs(x1 - worldX) * 2 <= (size + Size) &&
		abs(y1 - worldY) * 2 <= (size + Size)
		);
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