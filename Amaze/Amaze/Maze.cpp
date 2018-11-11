#include "Maze.h"



Maze::Maze(PhysicsSystem * physics) :
	_physics(physics)
{
}


Maze::~Maze()
{
}

//size is used to generate a maze of size by size dimension
void Maze::GenerateMaze(int size) {
	_size = size; //private member other functions can access for the maze size, saves passing in repeatedly as an argument
	//our return list containing all the walls
	if (_maze == nullptr) _maze = std::make_unique<std::vector<std::unique_ptr<GridBlock>>>();

	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (i * j <= _maze->size()) {
				auto gridBlock = std::make_unique<GridBlock>(j, i, (GridBlock::WALL_LENGTH)*j, GridBlock::WALL_LENGTH*i);
				gridBlock->Colour = sf::Color(255, 0, 0);
				gridBlock->RenderColour = gridBlock->Colour;
				_maze->push_back(std::move(gridBlock));
			}
			else
			{
				auto block = MultiHack(i, j);
				block->Enable(true);
			}
		}
	}

	_start = std::make_unique<GridLocation>();
	_finish = std::make_unique<GridLocation>();
	CreateStartAndFinishLocations(*_start, *_finish);
	CreateStart(_start->X, _start->Y);
	CreateFinish(_finish->X, _finish->Y);
	CreateRoute(*_start, *_finish);

	//proceed to create false routes from the start -- these lead the player to other deadends
	_deadends = CreateFauxRoutes(_size / 2);
}

//takes two GridLocations as input by reference and populates them with the (x,y) in the maze of their respective positions
void Maze::CreateStartAndFinishLocations(GridLocation &start, GridLocation &finish)
{
	//now randomly select two chambers with sufficient distance to be the start and the end
	//initialise everything to 0
	start.X = start.Y = finish.Y = finish.X = 0;

	//try to find a satisfactory start and finish position
	//satisfactory = distance between the two is >= MIN_DISTANCE
	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		start.X = CalcRandomPoint();
		start.Y = CalcRandomPoint();
		//possible save cpu by doing start once and then just varying finish?
		finish.X = CalcRandomPoint();
		finish.Y = CalcRandomPoint();

		if (MeetsConstraints(start, finish))
		{
			//found satisfactory coords, SCARPER!!!
			break;
		}
	}
}

void Maze::CreateStart(int x, int y)
{
	//mak = multidimension array hack, allows access to the deque in a way similar to accessing a 2d array
	_startPoint = MultiHack(x, y);
	_startPoint->MakeStart();
	DetectCollidibles(*_startPoint);
	_physics->RemoveCollidable(_startPoint);
}

void Maze::CreateFinish(int x, int y)
{
	//mak = multidimension array hack
	_finishPoint = MultiHack(x, y);
	_finishPoint->MakeFinish();
	DetectCollidibles(*_finishPoint);
	_physics->RemoveCollidable(_finishPoint);
}

std::vector<std::unique_ptr<GridLocation>> Maze::CreateFauxRoutes(unsigned int amount) {
	std::vector<std::unique_ptr<GridLocation>> deadends;

	//generate the given amount of routes
	while (deadends.size() < amount) {
		auto temp = CreateDeadend(*_start); //this should get refactored, complexity is growing, old code isn't being updated to reflect the changes
		//deadends will be empty at first so just add the first generated value which meets contraints
		if (deadends.size() < 1) {
			deadends.push_back(std::move(temp));
		}
		else {
			//make sure they are unique (ie. not the same as previously generated routes)
			bool found = false;
			for (unsigned int i = 0; i < deadends.size(); i++)
			{
				if (deadends.at(i)->X == temp->X && deadends.at(i)->Y == temp->Y)
				{
					//GridLocation is already a deadend
					//start again
					found = true;
					break;
				}
			}
			//gridlocation wasn't found so make sure it meets constraints
			if (!found) {
				if (MeetsConstraints(*temp, *_start))
				{
					//if everything is cool, add it to the list
					deadends.push_back(std::move(temp));
				}
			}
		}
	}

	//create routes
	for (unsigned int i = 0; i < deadends.size(); i++)
	{
		CreateRoute(*_start, *deadends.at(i));
	}

	return deadends;
}

std::unique_ptr<GridLocation> Maze::CreateDeadend(GridLocation & finish) {
	auto temp = std::make_unique<GridLocation>();
	temp->X = temp->Y = 0;

	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		temp->X = CalcRandomPoint();
		temp->Y = CalcRandomPoint();

		if (MeetsConstraints(*temp, finish)) {
			//done
			break;
		}
	}

	auto cached = MultiHack(temp->X, temp->Y);
	//make the deadend empty
	cached->Enable(false);
	DetectCollidibles(*cached);
	_physics->RemoveCollidable(cached);
	return temp;
}

void Maze::CreateRoute(GridLocation a, GridLocation b)
{
	//mister gorbachev, tear down theses wall!
	if (a.X == b.X && a.Y == b.Y) return;

	//wrap around.
	if (a.X > _size) a.X = 0;
	if (a.Y > _size) a.Y = 0;

	if (a.X < b.X) {
		MarkPath(a.X, a.Y);
		GridLocation temp;
		temp.X = a.X + 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}
	else if (a.X > b.X) {
		MarkPath(a.X, a.Y);
		GridLocation temp;
		temp.X = a.X - 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}
	else if (a.Y < b.Y) {
		MarkPath(a.X, a.Y);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y + 1;
		CreateRoute(temp, b);
	}
	else if (a.Y > b.Y)
	{
		MarkPath(a.X, a.Y);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y - 1;
		CreateRoute(temp, b);
	}
}

void Maze::DetectCollidibles(GridBlock & a) {
	// _____________
	// |___|___|___|
	// |___|_a_|_#_|
	// |___|___|___|
	//right
	auto xplus1 = MultiHack(a.X + 1, a.Y);
	MakeCollide(xplus1);

	// _____________
	// |___|___|___|
	// |_#_|_a_|___|
	// |___|___|___|
	//left
	auto xminus1 = MultiHack(a.X - 1, a.Y);
	MakeCollide(xminus1);

	// _____________
	// |___|_#_|___|
	// |___|_a_|___|
	// |___|___|___|
	//up
	auto yplus1 = MultiHack(a.X, a.Y + 1);
	MakeCollide(yplus1);

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |___|_#_|___|
	//down
	auto yminus1 = MultiHack(a.X, a.Y - 1);
	MakeCollide(yminus1);
	// _____________
	// |_#_|___|___|
	// |___|_a_|___|
	// |___|___|___|
	//up and left
	auto upleft = MultiHack(a.X - 1, a.Y + 1);
	MakeCollide(upleft);

	// _____________
	// |___|___|_#_|
	// |___|_a_|___|
	// |___|___|___|
	//up and right
	auto upright = MultiHack(a.X + 1, a.Y + 1);
	MakeCollide(upright);

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |___|___|_#_|
	//down and right
	auto downright = MultiHack(a.X + 1, a.Y - 1);
	MakeCollide(downright);

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |_#_|___|___|
	//down and left
	auto downleft = MultiHack(a.X - 1, a.Y - 1);
	MakeCollide(downleft);
	
}

// Checks to see if the given locations meet the required constrains
bool Maze::MeetsConstraints(GridLocation & one, GridLocation & two)
{
	const int MIN_DISTANCE = (_size - 2) / 2;

	if (FindDistance(one, two) >= MIN_DISTANCE) {
		return true;
	}

	return false;
}

int Maze::FindDistance(GridLocation & one, GridLocation & two) {
	int xDifference = two.X - one.X;
	int yDifference = two.Y - one.Y;
	//a^2 + b^2 = c^2
	return (int)sqrt((xDifference*xDifference) + (yDifference*yDifference));
}

GridBlock * Maze::MultiHack(int x, int y)
{
	if ((x + (y * _size)) >= (_size*_size)) return nullptr;
	if ((x + (y*_size)) < 0) return nullptr;
	if (_maze->empty()) return nullptr;

	return _maze->at(x + (y * _size)).get();
}

void Maze::Render(sf::RenderWindow * window, int bubbleSize, Pawn * hero)
{
	//cycle through and render all the walls of the maze	
	for (auto iter = _maze->begin(); iter != _maze->end(); iter++)
	{
		float dist = DistanceToHero((*iter).get(), hero);
		float inter = 1.0f - (dist / bubbleSize);

		if (inter < 0) continue;

		(*iter)->Colour.r *= inter;
		(*iter)->Colour.g *= inter;
		(*iter)->Colour.b *= inter;

		(*iter)->Render(window);

		(*iter)->Colour = (*iter)->RenderColour;
	}
}

GridBlock * Maze::GetFinish() const
{
	return _finishPoint;
}

GridBlock * Maze::GetStart() const
{
	return _startPoint;
}

int Maze::GetSize() const
{
	return _size;
}

const std::vector<std::unique_ptr<GridLocation>>& Maze::GetDeadends() const
{
	return _deadends;
}

const GridBlock & Maze::GetBlock(int x, int y)
{
	return *(MultiHack(x, y));
}

float Maze::DistanceToHero(Pawn * pawn, Pawn * hero)
{
	float x = hero->WorldX - pawn->WorldX;
	float y = hero->WorldY - pawn->WorldY;

	return sqrtf((x*x) + (y*y));
}

int Maze::CalcRandomPoint() const
{
	return 1 + rand() % (_size - 2);
}

void Maze::MakeCollide(GridBlock * block)
{
	if (block != nullptr && !block->IsStart() && !block->IsFinish() && block->IsEnabled()) {
		_physics->AddCollidable(block);
		block->IsCollidable();
	}
}

void Maze::MarkPath(int x, int y)
{
	auto block = MultiHack(x ,y);
	block->Enable(false);
	DetectCollidibles(*block);
	_physics->RemoveCollidable(block);
}
