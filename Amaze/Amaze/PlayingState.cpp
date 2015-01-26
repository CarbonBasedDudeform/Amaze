#include "PlayingState.h"

PlayingState::PlayingState()
{
	//seed rand to make everything more unique for each playingstate
	srand((unsigned int)time(NULL));
	_hero = new HeroPawn();
}


PlayingState::~PlayingState()
{
	delete _hero;
	delete _heroController;
}

void PlayingState::Init() {
	//Init code
	GenerateMaze(10);
}

//size is used to generate a maze of size by size dimension
void PlayingState::GenerateMaze(int size) {
	_size = size; //private member other functions can access for the maze size, saves passing in repeatedly as an argument
	//our return list containing all the walls
	_maze = new std::deque<GridBlock *>();
	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			_maze->push_back(new GridBlock(i,j, GridBlock::WALL_LENGTH*i, GridBlock::WALL_LENGTH*j));
		}
	}

	_start = new GridLocation();
	_finish = new GridLocation();
	CreateStartAndFinishLocations(*_start, *_finish);
	CreateStart(_start->X, _start->Y);
	CreateFinish(_finish->X, _finish->Y);

	CreateRoute(*_start, *_finish);

	//proceed to create 3 false routes from the start -- these lead the player to other deadends
	CreateFauxRoutes(15);

}

//takes two GridLocations as input by reference and populates them with the (x,y) in the maze of their respective positions
void PlayingState::CreateStartAndFinishLocations(GridLocation &start, GridLocation &finish)
{
	//now randomly select two chambers with sufficient distance to be the start and the end
	//initialise everything to 0
	start.X = start.Y = finish.Y = finish.X = 0;

	//try to find a satisfactory start and finish position
	//satisfactory = distance between the two is >= MIN_DISTANCE
	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		start.X = rand() % _size;
		start.Y = rand() % _size;
		//possible save cpu by doing start once and then just varying finish?
		finish.X = rand() % _size;
		finish.Y = rand() % _size;

		if (MeetsConstraints(start, finish))
		{
			//found satisfactory coords, SCARPER!!!
			break;
		}
	}
}

void PlayingState::CreateStart(int x, int y)
{
	//mak = multidimension array hack, allows access to the deque in a way similar to accessing a 2d array
	int mah = y + (x * _size);
	auto cached = _maze->at(mah);
	cached->MakeStart();
	_hero->WorldX = cached->WorldX;
	_hero->WorldY = cached->WorldY;

	//:::::::::::::::IMPORTANT::::::::::::::: HeroController is created here so that the camera can be focused on the hero pawn correctly.
	//										 aka, herocontroller needs to be created after the pawns WorldX and Y have been set
	_heroController = new HeroController(_hero);
}

void PlayingState::CreateFinish(int x, int y)
{
	//mak = multidimension array hack
	int mah = y + (x * _size);
	_maze->at(mah)->MakeFinish();
}

void PlayingState::CreateFauxRoutes(unsigned int amount) {
	auto deadends = new std::deque<GridLocation *>();
	//generate the given amount of routes
	while (deadends->size() < amount) {
		GridLocation * temp = CreateDeadend(*_start); //this should get refactored, complexity is growing, old code isn't being updated to reflect the changes
		//deadends will be empty at first so just add the first generated value which meets contraints
		if (deadends->size() < 1) {
			deadends->push_back(temp);
		}
		else {
			//make sure they are unique (ie. not the same as previously generated routes)
			bool found = false;
			for (unsigned int i = 0; i < deadends->size(); i++)
			{
				if (deadends->at(i)->X == temp->X && deadends->at(i)->Y == temp->Y)
				{
					//GridLocation is already a deadend
					//start again
					delete temp;
					found = true;
					break;
				}
			}
			//gridlocation wasn't found so make sure it meets constraints
			if (!found) {
				if (MeetsConstraints(*temp, *_start))
				{
					//if everything is cool, add it to the list
					deadends->push_back(temp);
				}
				else {
					//otherwise scrap it and start again
					delete temp;
				}
			}
		}
	}

	//create routes
	for (unsigned int i = 0; i < deadends->size(); i++)
	{
		CreateRoute(*_start, *deadends->at(i));
	}
}

GridLocation * PlayingState::CreateDeadend(GridLocation & finish) {
	GridLocation * temp = new GridLocation();
	temp->X = temp->Y = 0;

	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		temp->X = rand() % _size;
		temp->Y = rand() % _size;

		if (MeetsConstraints(*temp, finish)) {
			//done
			break;
		}
	}

	return temp;
}

//Checks to see if the given locations meet the required constrains
bool PlayingState::MeetsConstraints(GridLocation & one, GridLocation & two)
{
	const int MIN_DISTANCE = _size / 2;

	if (FindDistance(one, two) >= MIN_DISTANCE) {
		return true;
	}

	return false;
}

int PlayingState::FindDistance(GridLocation & one, GridLocation & two){
	int xDifference = two.X - one.X;
	int yDifference = two.Y - one.Y;
			//a^2 + b^2 = c^2
	return (int)sqrt((xDifference*xDifference) + (yDifference*yDifference));
}

void PlayingState::CreateRoute(GridLocation & a, GridLocation & b)
{
	//mister gorbachev, tear down theses wall!
	if (a.X == b.X && a.Y == b.Y) return;

	if (a.X < b.X) {
		_maze->at(a.Y + (a.X * 10))->EnableRight(false);
		_maze->at(a.Y + (a.X * 10))->EnableLeft(false);

		GridLocation temp;
		temp.X = a.X + 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}
	else if (a.X > b.X) {
		_maze->at(a.Y + (a.X * 10))->EnableRight(false);
		_maze->at(a.Y + (a.X * 10))->EnableLeft(false);
		GridLocation temp;
		temp.X = a.X - 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}
	else if (a.Y < b.Y) {
		_maze->at(a.Y + (a.X * 10))->EnableTop(false);
		_maze->at(a.Y + (a.X * 10))->EnableBottom(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y + 1;
		CreateRoute(temp, b);
	}
	else if (a.Y > b.Y)
	{
		_maze->at(a.Y + (a.X * 10))->EnableTop(false);
		_maze->at(a.Y + (a.X * 10))->EnableBottom(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y - 1;
		CreateRoute(temp, b);
	}

	if (a.X == b.X) {
		_maze->at(a.Y + (a.X * 10))->EnableRight(false);
		_maze->at(a.Y + (a.X * 10))->EnableLeft(false);
	}
}

void PlayingState::Render(sf::RenderWindow * window) {
	//set the camera
	window->setView(*_heroController->GetView());

	//cycle through and render all the walls of the maze	
	for (unsigned int i = 0; i < _maze->size(); i++)
	{
		_maze->at(i)->Render(window);
	}

	_hero->Render(window);
}

void PlayingState::ProcessInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_heroController->MoveLeft();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_heroController->MoveRight();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_heroController->MoveUp();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_heroController->MoveDown();
	}
}