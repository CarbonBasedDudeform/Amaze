#pragma once
#include "PhysicsSystem.h"

class Controller
{
public:
	Controller();
	virtual ~Controller();
	virtual void Process(BlockedDirections, float timeDelta) = 0;
protected:
	float _speed;
};

