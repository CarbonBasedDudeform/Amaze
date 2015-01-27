#pragma once
#include "Controller.h"
#include "AIPawn.h"
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
};

