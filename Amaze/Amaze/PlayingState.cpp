#include "PlayingState.h"

PlayingState::PlayingState()
{
}


PlayingState::~PlayingState()
{
}

void PlayingState::Init() {
	//Init code
	_maze = GenerateMaze(10);
}

//size is used to generate a maze of size by size dimension
std::deque<GridBlock *> * PlayingState::GenerateMaze(int size) {
	_size = size; //private member other functions can access for the maze size, saves passing in repeatedly as an argument
	//our return list containing all the walls
	auto maze = new std::deque<GridBlock *>();
	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			maze->push_back(new GridBlock(i,j, GridBlock::WALL_LENGTH*i, GridBlock::WALL_LENGTH*j));
		}
	}

	GridLocation start;
	GridLocation finish;
	CreateStartAndFinishLocations(start, finish);
	//cheeky little hack for multidimensional access, 
	maze->at(start.Y+(start.X*size))->MakeStart();
	maze->at(finish.Y + (finish.X*size))->MakeFinish();

	CreateRoute(start, finish, *maze);
	return maze;
}

//takes two GridLocations as input by reference and populates them with the (x,y) in the maze of their respective positions
void PlayingState::CreateStartAndFinishLocations(GridLocation &start, GridLocation &finish)
{
	//now randomly select two chambers with sufficient distance to be the start and the end
	srand(time(NULL));
	//initialise everything to 0
	start.X = start.Y = finish.Y = finish.X = 0;

	const int MAX_ATTEMPTS = 10;

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
	return sqrt((xDifference*xDifference) + (yDifference*yDifference));
}

void PlayingState::CreateRoute(GridLocation & a, GridLocation & b, std::deque<GridBlock*> & maze)
{
	//mister gorbachev, tear down theses wall!
	if (a.X == b.X && a.Y == b.Y) return;

	if (a.X < b.X) {
		maze.at(a.Y + (a.X * 10))->EnableRight(false);
		maze.at(a.Y + (a.X * 10))->EnableLeft(false);

		GridLocation temp;
		temp.X = a.X + 1;
		temp.Y = a.Y;
		CreateRoute(temp, b, maze);
	}
	else if (a.X > b.X) {
		maze.at(a.Y + (a.X * 10))->EnableRight(false);
		maze.at(a.Y + (a.X * 10))->EnableLeft(false);
		GridLocation temp;
		temp.X = a.X - 1;
		temp.Y = a.Y;
		CreateRoute(temp, b, maze);
	}
	else if (a.Y < b.Y) {
		maze.at(a.Y + (a.X * 10))->EnableTop(false);
		maze.at(a.Y + (a.X * 10))->EnableBottom(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y + 1;
		CreateRoute(temp, b, maze);
	}
	else if (a.Y > b.Y)
	{
		maze.at(a.Y + (a.X * 10))->EnableTop(false);
		maze.at(a.Y + (a.X * 10))->EnableBottom(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y - 1;
		CreateRoute(temp, b, maze);
	}

	if (a.X == b.X) {
		maze.at(a.Y + (a.X * 10))->EnableRight(false);
		maze.at(a.Y + (a.X * 10))->EnableLeft(false);
	}
}

void PlayingState::Render(sf::RenderWindow * window) {
	//cycle through and render all the walls of the maze	
	for (int i = 0; i < _maze->size(); i++)
	{
		_maze->at(i)->Render(window);
	}
}