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
	//our return list containing all the walls
	auto maze = new std::deque<GridBlock *>();
	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			maze->push_back(new GridBlock(i,j, GridBlock::WALL_WIDTH*i*6, GridBlock::WALL_LENGTH*j));
		}
	}

	GridLocation start;
	GridLocation finish;
	CreateStartAndFinishLocations(start, finish, size);
	//cheeky little hack for multidimensional access, 
	maze->at(start.Y+(start.X*size))->MakeStart();
	maze->at(finish.Y + (finish.X*size))->MakeFinish();

	return maze;
}

//takes two GridLocations as input by reference and populates them with the (x,y) in the maze of their respective positions
void PlayingState::CreateStartAndFinishLocations(GridLocation &start, GridLocation &finish, int size)
{
	//now randomly select two chambers with sufficient distance to be the start and the end
	const int MIN_DISTANCE = size / 2;
	srand(time(NULL));
	//initialise everything to 0
	start.X = start.Y = finish.Y = finish.X = 0;

	const int MAX_ATTEMPTS = 10;

	//try to find a satisfactory start and finish position
	//satisfactory = distance between the two is >= MIN_DISTANCE
	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		start.X = rand() % size;
		start.Y = rand() % size;
		//possible save cpu by doing start once and then just varying finish?
		finish.X = rand() % size;
		finish.Y = rand() % size;

		if (FindDistance(start, finish) >= MIN_DISTANCE)
		{
			//found satisfactory coords, SCARPER!!!
			break;
		}
	}
}

int PlayingState::FindDistance(GridLocation & one, GridLocation & two){
	int xDifference = two.X - one.X;
	int yDifference = two.Y - one.Y;
			//a^2 + b^2 = c^2
	return sqrt((xDifference*xDifference) + (yDifference*yDifference));
}

void PlayingState::Render(sf::RenderWindow * window) {
	//cycle through and render all the walls of the maze	
	for (int i = 0; i < _maze->size(); i++)
	{
		_maze->at(i)->Render(window);
	}
}