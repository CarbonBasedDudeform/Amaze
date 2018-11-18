#include "AIController.h"

AIController::AIController(std::vector<std::unique_ptr<AIPawn>> * pawns, Maze * maze, PhysicsSystem * physics, HeroPawn * hero)
{
	_physics = physics;
	_hero = hero;
	_maze = maze;
	_pawns = std::make_unique<std::vector<std::unique_ptr<AIPawnWrapper>>>();

	auto& deadends = _maze->GetDeadends();

	auto deadendCount = deadends.size();
	int i = 0;
	for (auto iter = pawns->begin(); iter != pawns->end(); iter++)
	{
		auto temp = std::make_unique<AIPawnWrapper>();
		temp->pawn = (*iter).get();
		auto & block = _maze->GetBlock(deadends.at(i % deadendCount)->X, deadends.at(i % deadendCount)->Y);
		temp->pawn->WorldX = block.WorldX;
		temp->pawn->WorldY = block.WorldY;
		_physics->AddCollidable(temp->pawn);
		_pawns->push_back(std::move(temp));
		i++;
	}

	_speed = 0.05f;
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
		(*iter)->MyIntention = std::move(DecideIntent((*iter).get(), (*iter)->MyIntention));
		//and move
		MoveIntoSpace((*iter).get(), timeDelta);
	}
}

Intention AIController::DecideIntent(AIPawnWrapper * pawn, Intention previousIntent) 
{	
	auto dist = DistanceToHero(pawn->pawn, _hero);
	const bool HeroHasBeenSeen = dist < VIEW_DISTANCE;
	const bool SearchMore = previousIntent.Searching > 0 && previousIntent.Searching <= 5;
	const bool CloseToHero = dist < 40;
	if (HeroHasBeenSeen) {
		if (CloseToHero) {
			return std::move(ShootToKill());
		}

		_lastLocation.WorldX = _hero->WorldX;
		_lastLocation.WorldY = _hero->WorldY;
		auto intent = Investigate(pawn->pawn, previousIntent);
		intent.Shoot = true;
		return std::move(intent);
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
		if (!blocked.Up.Blocked && blocked.Up.Distance >= blocked.Down.Distance && blocked.Up.Distance >= blocked.Left.Distance && blocked.Up.Distance >= blocked.Right.Distance) {
			intent.Up = true;
			intentedDirections++;
		}
		else if (!blocked.Down.Blocked && blocked.Down.Distance >= blocked.Up.Distance && blocked.Down.Distance >= blocked.Left.Distance && blocked.Down.Distance >= blocked.Right.Distance) {
			intent.Down = true;
			intentedDirections++;
		}
		else if (!blocked.Left.Blocked && blocked.Left.Distance >= blocked.Down.Distance && blocked.Left.Distance >= blocked.Up.Distance && blocked.Left.Distance >= blocked.Right.Distance) {
			intent.Left = true;
			intentedDirections++;
		}
		else if (!blocked.Right.Blocked && blocked.Right.Distance >= blocked.Up.Distance && blocked.Right.Distance >= blocked.Left.Distance && blocked.Right.Distance >= blocked.Down.Distance) {
			intent.Right = true;
			intentedDirections++;
		}
	}
	else
	{
		if (!blocked.Left.Blocked && blocked.Left.Distance >= blocked.Down.Distance && blocked.Left.Distance >= blocked.Up.Distance && blocked.Left.Distance >= blocked.Right.Distance) {
			intent.Left = true;
			intentedDirections++;
		}
		else if (!blocked.Right.Blocked && blocked.Right.Distance >= blocked.Up.Distance && blocked.Right.Distance >= blocked.Left.Distance && blocked.Right.Distance >= blocked.Down.Distance) {
			intent.Right = true;
			intentedDirections++;
		}
		else if (!blocked.Up.Blocked && blocked.Up.Distance >= blocked.Down.Distance && blocked.Up.Distance >= blocked.Left.Distance && blocked.Up.Distance >= blocked.Right.Distance) {
			intent.Up = true;
			intentedDirections++;
		}
		else if (!blocked.Down.Blocked && blocked.Down.Distance >= blocked.Up.Distance && blocked.Down.Distance >= blocked.Left.Distance && blocked.Down.Distance >= blocked.Right.Distance) {
			intent.Down = true;
			intentedDirections++;
		}
	}

	if (intentedDirections == 0 || intentedDirections > 1) {
		enum DIR { UP, DOWN, LEFT, RIGHT };
		std::vector<DIR> PossibleDirections;
		auto isZero = intentedDirections == 0;
		if (intent.Up || isZero) {
			PossibleDirections.push_back(DIR::UP);
		}
		if (intent.Down || isZero) {
			PossibleDirections.push_back(DIR::DOWN);
		}
		if (intent.Left || isZero) {
			PossibleDirections.push_back(DIR::LEFT);
		}
		if (intent.Right || isZero) {
			PossibleDirections.push_back(DIR::RIGHT);
		}

		auto cap = PossibleDirections.size();
		auto r = std::rand() % cap;
		auto result = PossibleDirections.at(r);
		intent.Up = intent.Down = intent.Left = intent.Right = false;
		if (result == DIR::UP) intent.Up = true;
		if (result == DIR::DOWN) intent.Down = true;
		if (result == DIR::LEFT) intent.Left = true;
		if (result == DIR::RIGHT) intent.Right = true;
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

Intention AIController::ShootToKill()
{
	Intention temp;
	temp.Shoot = true;
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
	if (wrapper->MyIntention.Shoot) {
		sf::Vector2f dir(_lastLocation.WorldX - wrapper->pawn->WorldX,_lastLocation.WorldY - wrapper->pawn->WorldY);
		auto dist = DistanceToLocation(wrapper->pawn, _lastLocation);
		dir.x /= dist;
		dir.y /= dist;

		auto laser = std::make_unique<Laser>(wrapper->pawn->GetPosition(), dir, atan2(dir.y, dir.x) * 180 / 3.14f, "Textures/enemyLaser.png");
		laser->parent = wrapper->pawn;
		_physics->AddCollidable(laser.get());
		Lasers.push_back(std::move(laser));
	}
	if (wrapper->MyIntention.Left) MoveLeft(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Right) MoveRight(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Up) MoveUp(wrapper->pawn, timeDelta);
	if (wrapper->MyIntention.Down) MoveDown(wrapper->pawn, timeDelta);
}

bool AIController::MoveLeft(AIPawn * pawn, float timeDelta) {
	pawn->WorldX -= _speed * timeDelta;
	pawn->SetRotation(-90);
	pawn->Direction.x = -1;
	pawn->Direction.y = 0;
	return true;
}

bool AIController::MoveRight(AIPawn * pawn, float timeDelta) {
	pawn->WorldX += _speed * timeDelta;
	pawn->SetRotation(90);
	pawn->Direction.x = 1;
	pawn->Direction.y = 0;
	return true;
}

bool AIController::MoveUp(AIPawn * pawn, float timeDelta) {
	pawn->WorldY -= _speed * timeDelta;
	pawn->SetRotation(0);
	pawn->Direction.x = 0;
	pawn->Direction.y = -1;
	return true;
}

bool AIController::MoveDown(AIPawn * pawn, float timeDelta) {
	pawn->WorldY += _speed * timeDelta;
	pawn->SetRotation(180);
	pawn->Direction.x = 0;
	pawn->Direction.y = 1;
	return true;
}