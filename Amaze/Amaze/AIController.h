#pragma once
#include "Controller.h"
#include "AIPawn.h"
#include "PhysicsSystem.h"

#include <vector>

class AIController :
	public Controller
{
public:
	AIController(std::vector<AIPawn *> *);
	~AIController();
	void Think();

private:
	std::vector<AIPawn *> * _pawns;
	
	void FindOpenSpace();
	void MoveIntoSpace();

	void MoveLeft(AIPawn *);
	void MoveRight(AIPawn *);
	void MoveUp(AIPawn *);
	void MoveDown(AIPawn *);

	PhysicsSystem * _physics;
};

