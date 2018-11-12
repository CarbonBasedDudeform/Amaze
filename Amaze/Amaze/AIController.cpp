#include "AIController.h"

AIController::AIController(std::vector<std::unique_ptr<AIPawn>> * pawns, Maze * maze, PhysicsSystem * physics)
{
	_physics = physics;
	_maze = maze;
	_pawns = std::make_unique<std::vector<std::unique_ptr<AIPawnWrapper>>>();

	auto& deadends = _maze->GetDeadends();

	int i = 0;
	for (auto iter = pawns->begin(); iter != pawns->end(); iter++)
	{
		auto temp = std::make_unique<AIPawnWrapper>();
		temp->pawn = (*iter).get();
		auto & block = _maze->GetBlock(deadends.at(i)->X, deadends.at(i)->Y);
		temp->pawn->WorldX = block.WorldX;
		temp->pawn->WorldY = block.WorldY;
		_physics->AddCollidable(temp->pawn);
		_pawns->push_back(std::move(temp));
		i++;
	}

	_speed = 0.02f;
	_heroLocation = std::make_unique<Belief>();
	_heroLocation->X = rand() % _maze->GetSize();
	_heroLocation->Y = rand() % _maze->GetSize();
	_heroLocation->WorldX = (_heroLocation->X * GridBlock::WALL_LENGTH) + GridBlock::WALL_LENGTH / 2; //this is the equation to calculate world pos, the offset is the grid position * the length of a wall, plus half for the centre of the wall
	_heroLocation->WorldY = (_heroLocation->Y * GridBlock::WALL_LENGTH) + GridBlock::WALL_LENGTH / 2;
}

AIController::~AIController()
{
}

void AIController::Process(BlockedDirections blocked, float timeDelta) {
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
			(*iter)->MyIntention = DecideIntent((*iter).get(), (*iter)->MyIntention);
		}

		//and move
		MoveIntoSpace((*iter).get(), timeDelta);
	}
}

Intention AIController::DecideIntent(AIPawnWrapper * pawn, Intention previousIntent) {
	Intention intent;
	auto blocked = _physics->RayCastCollide(pawn->pawn, VIEW_DISTANCE);

	if (previousIntent.Down && !blocked.Down.Blocked ||
		previousIntent.Up && !blocked.Up.Blocked ||
		previousIntent.Left && !blocked.Left.Blocked ||
		previousIntent.Right && !blocked.Right.Blocked
		)
	{
		return previousIntent;
	}
	int rand = std::rand() % 100 - std::rand() % 100;
	int intentedDirections = 0;
	if (rand < 0) {
		if (blocked.Up.Distance >= blocked.Down.Distance && blocked.Up.Distance >= blocked.Left.Distance && blocked.Up.Distance >= blocked.Right.Distance) {
			intent.Up = true;
			intentedDirections++;
		}
		else if (blocked.Down.Distance >= blocked.Up.Distance && blocked.Down.Distance >= blocked.Left.Distance && blocked.Down.Distance >= blocked.Right.Distance) {
			intent.Down = true;
			intentedDirections++;
		}
		else if (blocked.Left.Distance >= blocked.Down.Distance && blocked.Left.Distance >= blocked.Up.Distance && blocked.Left.Distance >= blocked.Right.Distance) {
			intent.Left = true;
			intentedDirections++;
		}
		else if (blocked.Right.Distance >= blocked.Up.Distance && blocked.Right.Distance >= blocked.Left.Distance && blocked.Right.Distance >= blocked.Down.Distance) {
			intent.Right = true;
			intentedDirections++;
		}
	}
	else
	{
		if (blocked.Left.Distance >= blocked.Down.Distance && blocked.Left.Distance >= blocked.Up.Distance && blocked.Left.Distance >= blocked.Right.Distance) {
			intent.Left = true;
			intentedDirections++;
		}
		else if (blocked.Right.Distance >= blocked.Up.Distance && blocked.Right.Distance >= blocked.Left.Distance && blocked.Right.Distance >= blocked.Down.Distance) {
			intent.Right = true;
			intentedDirections++;
		}
		else if (blocked.Up.Distance >= blocked.Down.Distance && blocked.Up.Distance >= blocked.Left.Distance && blocked.Up.Distance >= blocked.Right.Distance) {
			intent.Up = true;
			intentedDirections++;
		}
		else if (blocked.Down.Distance >= blocked.Up.Distance && blocked.Down.Distance >= blocked.Left.Distance && blocked.Down.Distance >= blocked.Right.Distance) {
			intent.Down = true;
			intentedDirections++;
		}
	}

	if (intentedDirections == 0 || intentedDirections > 1) {
		intent.Up = intent.Down = intent.Left = intent.Right = false;
		intent.Up = true;
	}

	return intent;
}

void AIController::FindOpenSpace() {
}

void AIController::MoveIntoSpace(AIPawnWrapper * wrapper, float timeDelta) {
	if (wrapper->MyIntention.Left) MoveLeft(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Right) MoveRight(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Up) MoveUp(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Down) MoveDown(wrapper->pawn, timeDelta);
}

bool AIController::MoveLeft(AIPawn * pawn, float timeDelta) {
	pawn->WorldX -= _speed * timeDelta;
	return true;
}

bool AIController::MoveRight(AIPawn * pawn, float timeDelta) {
	pawn->WorldX += _speed * timeDelta;
	return true;
}

bool AIController::MoveUp(AIPawn * pawn, float timeDelta) {
	pawn->WorldY -= _speed * timeDelta;
	return true;
}

bool AIController::MoveDown(AIPawn * pawn, float timeDelta) {
	pawn->WorldY += _speed * timeDelta;
	return true;
}