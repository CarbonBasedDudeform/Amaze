#pragma once
#include "Pawn.h"

class PhysicsSystem
{
public:
	static PhysicsSystem * GetInstance();
	~PhysicsSystem();
	bool AreColliding(Pawn*, Pawn*);
	float FindDistance(Pawn*, Pawn*);
	bool AreColliding(float, float, int, Pawn*);
	float FindDistance(float, float, Pawn*);
private:
	static PhysicsSystem * _instance;
	PhysicsSystem();
};

