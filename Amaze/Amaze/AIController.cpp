#include "AIController.h"


AIController::AIController(std::vector<AIPawn *> * pawns)
{
	_pawns = pawns;
	_speed = 0.5f;
}


AIController::~AIController()
{
}

#include <iostream>
void AIController::Think() {
	//std::cout << "AI thinking..." << std::endl;
	//for each pawn
	//find open space
		//move into it
	FindOpenSpace();
}

void AIController::FindOpenSpace() {
	for (auto iter = _pawns->begin(); iter != _pawns->end(); iter++)
	{
		
	}
}

void AIController::MoveIntoSpace() {
	for (auto iter = _pawns->begin(); iter != _pawns->end(); iter++)
	{

	}
}

void AIController::MoveLeft(AIPawn * pawn) {
	pawn->WorldX -= _speed;
}

void AIController::MoveRight(AIPawn * pawn) {
	pawn->WorldX += _speed;
}

void AIController::MoveUp(AIPawn * pawn) {
	pawn->WorldY -= _speed;
}

void AIController::MoveDown(AIPawn * pawn) {
	pawn->WorldY += _speed;
}