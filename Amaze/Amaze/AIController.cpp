#include "AIController.h"


AIController::AIController(std::vector<AIPawn *> * pawns, int mazeSize)
{
	_pawns = new std::vector<AIPawnWrapper *>();

	for (auto iter = pawns->begin(); iter != pawns->end(); iter++)
	{
		auto temp = new AIPawnWrapper();
		temp->pawn = (*iter);
		_pawns->push_back(temp);
	}

	_speed = 0.5f;
	_heroLocation = new Belief();
	_heroLocation->X = rand() % mazeSize;
	_heroLocation->Y = rand() % mazeSize;
	_heroLocation->WorldX = (_heroLocation->X * GridBlock::WALL_LENGTH) + GridBlock::WALL_LENGTH / 2; //this is the equation to calculate world pos, the offset is the grid position * the length of a wall, plus half for the centre of the wall
	_heroLocation->WorldY = (_heroLocation->Y * GridBlock::WALL_LENGTH) + GridBlock::WALL_LENGTH / 2;
	_physics = PhysicsSystem::GetInstance();
}

AIController::~AIController()
{
}

void AIController::Process(BlockedDirections blocked) {
	//std::cout << "AI thinking..." << std::endl;
	//for each pawn
		//decide intent
			//move according to the intent
	if (_pawns->size() < 0) return;

	for (auto iter = _pawns->begin(); iter != _pawns->end(); iter++)
	{
		if (!(*iter)->PreviousMoveOK)
		{
			//decide new intention
			(*iter)->MyIntention = DecideIntent((*iter)->MyIntention);
		}

		//and move
		MoveIntoSpace((*iter));
	}
}

Intention AIController::DecideIntent(Intention previousIntent) {
	Intention intent;

	//try a new way, one that's not the same as the last
	if (previousIntent.Left) {
		int rand = std::rand() % 100;
		if (rand < 33) intent.Right = true;
		else if (rand < 66) intent.Up = true;
		else intent.Down = true;
	} else if (previousIntent.Right) {
		int rand = std::rand() % 100;
		if (rand < 33) intent.Left = true;
		else if (rand < 66) intent.Up = true;
		else intent.Down = true;
	} else if (previousIntent.Up) {
		int rand = std::rand() % 100;
		if (rand < 33) intent.Right = true;
		else if (rand < 66) intent.Left = true;
		else intent.Down = true;
	} else if (previousIntent.Down) {
		int rand = std::rand() % 100;
		if (rand < 33) intent.Right = true;
		else if (rand < 66) intent.Up = true;
		else intent.Left = true;
	}
	else {
		int rand = std::rand() % 100;
		if (rand < 25) intent.Right = true;
		else if (rand < 50) intent.Up = true;
		else if (rand < 75) intent.Left = true;
		else intent.Down = true;
	}

	return intent;
}

void AIController::FindOpenSpace() {
}

void AIController::MoveIntoSpace(AIPawnWrapper * wrapper) {
	if (wrapper->MyIntention.Left) MoveLeft(wrapper->pawn);
	if (wrapper->MyIntention.Right) MoveRight(wrapper->pawn);
	if (wrapper->MyIntention.Up) MoveUp(wrapper->pawn);
	if (wrapper->MyIntention.Down) MoveDown(wrapper->pawn);
}

bool AIController::MoveLeft(AIPawn * pawn) {
	auto blockedDirs = _physics->IsColliding(pawn);
	if (blockedDirs.Left) return false;

	pawn->WorldX -= _speed;
	return true;
}

bool AIController::MoveRight(AIPawn * pawn) {
	auto blockedDirs = _physics->IsColliding(pawn);
	if (blockedDirs.Right) return false;

	pawn->WorldX += _speed;
	return true;
}

bool AIController::MoveUp(AIPawn * pawn) {
	auto blockedDirs = _physics->IsColliding(pawn);
	if (blockedDirs.Up) return false;

	pawn->WorldY -= _speed;
	return true;
}

bool AIController::MoveDown(AIPawn * pawn) {
	auto blockedDirs = _physics->IsColliding(pawn);
	if (blockedDirs.Down) return false;

	pawn->WorldY += _speed;
	return true;
}