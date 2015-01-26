#pragma once
#include "Pawn.h"

class PhysicsSystem
{
public:
	static PhysicsSystem * GetInstance();
	~PhysicsSystem();
	bool AreColliding(Pawn*, Pawn*);
private:
	static PhysicsSystem * _instance;
	PhysicsSystem();
};

