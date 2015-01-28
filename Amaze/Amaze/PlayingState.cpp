#include "PlayingState.h"

PlayingState::PlayingState()
{
	//seed rand to make everything more unique for each playingstate
	srand((unsigned int)time(NULL));
	_hero = new HeroPawn();
	_physics = PhysicsSystem::GetInstance();

	_terrors = new std::vector<AIPawn *>();
	_terrors->push_back(new AIPawn());
	_terrors->push_back(new AIPawn());

	_terrorsController = new AIController(_terrors);
	_placedAI = 0;
}


PlayingState::~PlayingState()
{
	delete _hero;
	delete _heroController;
}

void PlayingState::Init() {
	//Init code
	GenerateMaze(20);
}

GridBlock * PlayingState::MultiHack(int x, int y)
{
	if ((x + (y * _size)) >= (_size*_size)) return nullptr;
	if ((x + (y*_size)) < 0) return nullptr;

	return _maze->at(x + (y * _size));
}

//size is used to generate a maze of size by size dimension
void PlayingState::GenerateMaze(int size) {
	_size = size; //private member other functions can access for the maze size, saves passing in repeatedly as an argument
	//our return list containing all the walls
	_maze = new std::vector<GridBlock *>();
	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			_maze->push_back(new GridBlock(j,i, (GridBlock::WALL_LENGTH)*j, GridBlock::WALL_LENGTH*i));
		}
	}

	_start = new GridLocation();
	_finish = new GridLocation();
	CreateStartAndFinishLocations(*_start, *_finish);
	CreateStart(_start->X, _start->Y);
	CreateFinish(_finish->X, _finish->Y);
	CreateRoute(*_start, *_finish);

	//proceed to create false routes from the start -- these lead the player to other deadends
	CreateFauxRoutes(_size/2);
	
	//clean up - get rid of unrendered blocks
	CleanUp();
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
	auto cached = MultiHack(x, y);
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
	MultiHack(x,y)->MakeFinish();
}

void PlayingState::CreateFauxRoutes(unsigned int amount) {
	auto deadends = new std::vector<GridLocation *>();
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

	auto cached = MultiHack(temp->X, temp->Y);
	//make the deadend empty
	cached->Enable(false);

	//place the AIs (the terrors) at the deadends
	if (_placedAI != _terrors->size())
	{
		_terrors->at(_placedAI)->WorldX = cached->WorldX;
		_terrors->at(_placedAI)->WorldY = cached->WorldY;
		_placedAI++;
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

void PlayingState::CreateRoute(GridLocation a, GridLocation b)
{
	//mister gorbachev, tear down theses wall!
	if (a.X == b.X && a.Y == b.Y) return;

	//wrap around.
	if (a.X > _size) a.X = 0;
	if (a.Y > _size) a.Y = 0;

	if (a.X < b.X ) {
		MultiHack(a.X, a.Y)->Enable(false);
		GridLocation temp;
		temp.X = a.X + 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	} else if (a.X > b.X) {
		MultiHack(a.X, a.Y)->Enable(false);
		GridLocation temp;
		temp.X = a.X - 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}else if (a.Y < b.Y) {
		MultiHack(a.X, a.Y)->Enable(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y + 1;
		CreateRoute(temp, b);
	} else if (a.Y > b.Y)
	{
		MultiHack(a.X, a.Y)->Enable(false);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y - 1;
		CreateRoute(temp, b);
	}
}

void PlayingState::DetectCollidibles(GridBlock & a) {
	auto xplus1 = MultiHack(a.X + 1, a.Y);
	if (xplus1 != nullptr && !xplus1->IsStart()) {
		_physics->AddCollidable(xplus1);
		xplus1->IsCollidable();
	}
	auto xminus1 = MultiHack(a.X - 1, a.Y);
	if (xminus1 != nullptr && !xminus1->IsStart()) {
		_physics->AddCollidable(xminus1);
		xminus1->IsCollidable();
	}
	auto yplus1 = MultiHack(a.X, a.Y + 1);
	if (yplus1 != nullptr && !yplus1->IsStart()) {
		_physics->AddCollidable(yplus1);
		yplus1->IsCollidable();
	}
	auto yminus1 = MultiHack(a.X, a.Y - 1);
	if (yminus1 != nullptr && !yminus1->IsStart()){
		_physics->AddCollidable(yminus1);
		yminus1->IsCollidable();
	}

}

void PlayingState::CleanUp()
{
	//identify the indices of parts of the maze which can be removed (because they are not displayed and not the start or finish)
	std::vector<int> indices;
	for (int i = 0; i < _maze->size(); i++)
	{
		if (!_maze->at(i)->IsEnabled() && !_maze->at(i)->IsStart() && !_maze->at(i)->IsFinish())
		{
			DetectCollidibles(*_maze->at(i));
			_physics->CleanUp(_maze->at(i));
			indices.push_back(i);
		}
	}

	//remove the parts of the maze here because if it is done above then the size of maze decreases with each removal and elements get
	//reshuffled causing problems with using the multidimenional access hack.
	for (int i = 0; i < indices.size(); i++)
	{
		//with each pass through the loop something is removed
		//because the indices is order from lowest to highest (ie. in the order 10, 15, 205, 6000) then you can assume
		//that each removal moves the indices down by one. thus after N removals, the indices will have moved down by N.
		//making the previous list, with 10 and 15 removed, become: 203, 5998
		delete _maze->at((indices.at(i) - i));
		_maze->erase(_maze->begin() + (indices.at(i) - i));
	}
}

void PlayingState::Render(sf::RenderWindow * window) {
	//set the camera
	window->setView(*_heroController->GetView());

	//cycle through and render all the walls of the maze	
	for (auto iter = _maze->begin(); iter != _maze->end(); iter++)
	{
		(*iter)->Render(window);
	}

	_hero->Render(window);

	for (auto iter = _terrors->begin(); iter != _terrors->end(); iter++)
	{
		(*iter)->Render(window);
	}
}

void PlayingState::ProcessInput() {
	bool leftBlocked = false;
	bool rightBlocked = false;
	bool topBlocked = false;
	bool bottomBlocked = false;

	auto blocked = _physics->IsColliding(_hero);

	if (!blocked.Left && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_heroController->MoveLeft();
	}
	if (!blocked.Right && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_heroController->MoveRight();
	}
	if (!blocked.Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_heroController->MoveUp();
	} 
	if (!blocked.Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_heroController->MoveDown();
	}

	_terrorsController->Think();
}