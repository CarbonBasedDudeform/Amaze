#include "AIController.h"


AIController::AIController(std::vector<AIPawn *> * pawns)
{
	_pawns = pawns;
}


AIController::~AIController()
{
}

#include <iostream>
void AIController::Think() {
	//std::cout << "AI thinking..." << std::endl;
}