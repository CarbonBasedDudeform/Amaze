#include "AIController.h"

AIController::AIController(std::vector<std::unique_ptr<AIPawn>> * pawns, Maze * maze, PhysicsSystem * physics, HeroPawn * hero)
{
	_physics = physics;
	_hero = hero;
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
		(*iter)->MyIntention = DecideIntent((*iter).get(), (*iter)->MyIntention);
		//and move
		MoveIntoSpace((*iter).get(), timeDelta);
	}
}

Intention AIController::DecideIntent(AIPawnWrapper * pawn, Intention previousIntent) 
{	
	auto dist = DistanceToHero(pawn->pawn, _hero);
	const bool HeroHasBeenSeen = dist < VIEW_DISTANCE;
	const bool SearchMore = previousIntent.Searching > 0 && previousIntent.Searching <= 5;
	if (HeroHasBeenSeen) {
		_lastLocation.WorldX = _hero->WorldX;
		_lastLocation.WorldY = _hero->WorldY;
		return std::move(Investigate(pawn->pawn, previousIntent));
	}
	else if (SearchMore) {
		_lastLocation = std::move(RandomLocationNear(_lastLocation));
		return std::move(Investigate(pawn->pawn, previousIntent));
	}

	auto blocked = _physics->RayCastCollide(pawn->pawn, BLOCKING_DISTANCE);
	return std::move(Explore(blocked, previousIntent));
}

Intention AIController::Explore(const BlockedDirections& blocked, Intention& previousIntent) const
{
	if (previousIntent.Down && !blocked.Down.Blocked ||
		previousIntent.Up && !blocked.Up.Blocked ||
		previousIntent.Left && !blocked.Left.Blocked ||
		previousIntent.Right && !blocked.Right.Blocked
		)
	{
		return std::move(previousIntent);
	}

	Intention intent;
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

	return std::move(intent);
}

Intention AIController::Investigate(Pawn * pawn, const Intention& previousIntent)
{
	Intention temp;
	temp.Searching = previousIntent.Searching + 1;

	if (_lastLocation.WorldX < pawn->WorldX) {
		temp.Left = true;
	}
	else
	{
		temp.Right = true;
	}

	if (_lastLocation.WorldY < pawn->WorldY) {
		temp.Up = true;
	}
	else
	{
		temp.Down = true;
	}

	return std::move(temp);
}

float AIController::DistanceToHero(Pawn * pawn, Pawn * hero)
{
	float x = hero->WorldX - pawn->WorldX;
	float y = hero->WorldY - pawn->WorldY;

	return sqrtf((x*x) + (y*y));
}

float AIController::DistanceToLocation(Pawn * pawn, Belief & location)
{
	float x = location.WorldX - pawn->WorldX;
	float y = location.WorldY - pawn->WorldY;

	return sqrtf((x*x) + (y*y));
}

Belief AIController::RandomLocationNear(Belief & lastBelief)
{
	Belief temp;
	temp.WorldX = lastBelief.WorldX + (rand() % VIEW_DISTANCE);
	temp.WorldY = lastBelief.WorldY + (rand() % VIEW_DISTANCE);
	return std::move(temp);
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