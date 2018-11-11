#include "PlayingState.h"

PlayingState::PlayingState()
	: _finishPoint(nullptr)
{
	//seed rand to make everything more unique for each playingstate
	srand((unsigned int)time(NULL));
	_hero = std::make_unique<HeroPawn>();
	_physics = std::make_unique<PhysicsSystem>();

	//_terrors = new std::vector<AIPawn *>();
	//_terrors->push_back(new AIPawn());
	//_terrors->push_back(new AIPawn());

	_placedAI = 0;
}


PlayingState::~PlayingState()
{
}

void PlayingState::Init(PlayingStateOptions opts) {
	//Init code
	GenerateMaze(opts.LevelSize);
	//_terrorsController = new AIController(_terrors, mazeSize);
}

GridBlock * PlayingState::MultiHack(int x, int y)
{
	if ((x + (y * _size)) >= (_size*_size)) return nullptr;
	if ((x + (y*_size)) < 0) return nullptr;

	return _maze->at(x + (y * _size)).get();
}

//size is used to generate a maze of size by size dimension
void PlayingState::GenerateMaze(int size) {
	_size = size; //private member other functions can access for the maze size, saves passing in repeatedly as an argument
	//our return list containing all the walls
	_maze = std::make_unique<std::vector<std::unique_ptr<GridBlock>>>();
	//populate
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			auto gridBlock = std::make_unique<GridBlock>(j, i, (GridBlock::WALL_LENGTH)*j, GridBlock::WALL_LENGTH*i);
			gridBlock->Colour = sf::Color(255, 0, 0);
			gridBlock->RenderColour = gridBlock->Colour;
			_physics->AddCollidable(gridBlock.get());
			_maze->push_back(std::move(gridBlock));
		}
	}

	_start = std::make_unique<GridLocation>();
	_finish = std::make_unique<GridLocation>();
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
	_heroController = std::make_unique<HeroController>(_hero.get());
}

void PlayingState::CreateFinish(int x, int y)
{
	//mak = multidimension array hack
	_finishPoint = MultiHack(x, y);
	_finishPoint->MakeFinish();
	_physics->RemoveCollidable(_finishPoint);
}

void PlayingState::CreateFauxRoutes(unsigned int amount) {
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
}

std::unique_ptr<GridLocation> PlayingState::CreateDeadend(GridLocation & finish) {
	auto temp = std::make_unique<GridLocation>();
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
	_physics->RemoveCollidable(cached);
	//place the AIs (the terrors) at the deadends
	/*
	if (_placedAI != _terrors->size())
	{
		_terrors->at(_placedAI)->WorldX = cached->WorldX;
		_terrors->at(_placedAI)->WorldY = cached->WorldY;
		_placedAI++;
	}
	*/
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
		auto block = MultiHack(a.X, a.Y);
		block->Enable(false);
		_physics->RemoveCollidable(block);
		GridLocation temp;
		temp.X = a.X + 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	} else if (a.X > b.X) {
		auto block = MultiHack(a.X, a.Y);
		block->Enable(false);
		_physics->RemoveCollidable(block);
		GridLocation temp;
		temp.X = a.X - 1;
		temp.Y = a.Y;
		CreateRoute(temp, b);
	}else if (a.Y < b.Y) {
		auto block = MultiHack(a.X, a.Y);
		block->Enable(false);
		_physics->RemoveCollidable(block);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y + 1;
		CreateRoute(temp, b);
	} else if (a.Y > b.Y)
	{
		auto block = MultiHack(a.X, a.Y);
		block->Enable(false);
		_physics->RemoveCollidable(block);
		GridLocation temp;
		temp.X = a.X;
		temp.Y = a.Y - 1;
		CreateRoute(temp, b);
	}
}

void PlayingState::DetectCollidibles(GridBlock & a) {
	// _____________
	// |___|___|___|
	// |___|_a_|_#_|
	// |___|___|___|
	//right
	auto xplus1 = MultiHack(a.X + 1, a.Y);
	if (xplus1 != nullptr && !xplus1->IsStart() && !xplus1->IsFinish()) {
		_physics->AddCollidable(xplus1);
		xplus1->IsCollidable();
	}

	// _____________
	// |___|___|___|
	// |_#_|_a_|___|
	// |___|___|___|
	//left
	auto xminus1 = MultiHack(a.X - 1, a.Y);
	if (xminus1 != nullptr && !xminus1->IsStart() && !xminus1->IsFinish()) {
		_physics->AddCollidable(xminus1);
		xminus1->IsCollidable();
	}

	// _____________
	// |___|_#_|___|
	// |___|_a_|___|
	// |___|___|___|
	//up
	auto yplus1 = MultiHack(a.X, a.Y + 1);
	if (yplus1 != nullptr && !yplus1->IsStart() && !yplus1->IsFinish()) {
		_physics->AddCollidable(yplus1);
		yplus1->IsCollidable();
	}

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |___|_#_|___|
	//down
	auto yminus1 = MultiHack(a.X, a.Y - 1);
	if (yminus1 != nullptr && !yminus1->IsStart() && !yminus1->IsFinish()){
		_physics->AddCollidable(yminus1);
		yminus1->IsCollidable();
	}

	// _____________
	// |_#_|___|___|
	// |___|_a_|___|
	// |___|___|___|
	//up and left
	auto upleft = MultiHack(a.X - 1, a.Y + 1);
	if (upleft != nullptr && !upleft->IsStart() && !upleft->IsFinish())
	{
		_physics->AddCollidable(upleft);
		upleft->IsCollidable();
	}

	// _____________
	// |___|___|_#_|
	// |___|_a_|___|
	// |___|___|___|
	//up and right
	auto upright = MultiHack(a.X + 1, a.Y + 1);
	if (upright != nullptr && !upright->IsStart() && !upright->IsFinish())
	{
		_physics->AddCollidable(upright);
		upright->IsCollidable();
	}

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |___|___|_#_|
	//down and right
	auto downright = MultiHack(a.X + 1, a.Y - 1);
	if (downright != nullptr && !downright->IsStart() && !downright->IsFinish())
	{
		_physics->AddCollidable(downright);
		downright->IsCollidable();
	}

	// _____________
	// |___|___|___|
	// |___|_a_|___|
	// |_#_|___|___|
	//down and left
	auto downleft = MultiHack(a.X - 1, a.Y - 1);
	if (downleft != nullptr && !downleft->IsStart() && !downleft->IsFinish())
	{
		_physics->AddCollidable(downleft);
		downleft->IsCollidable();
	}
}

void PlayingState::CleanUp()
{
	//identify the indices of parts of the maze which can be removed (because they are not displayed and not the start or finish)
	/*std::vector<int> indices;
	for (int i = 0; i < _maze->size(); i++)
	{
		if (!_maze->at(i)->IsEnabled())
		{
			DetectCollidibles(*_maze->at(i));
			if (!_maze->at(i)->IsStart() && !_maze->at(i)->IsFinish())
			{
				_physics->CleanUp(_maze->at(i));
				indices.push_back(i);
			}
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
		//delete _maze->at((indices.at(i) - i));
		//_maze->erase(_maze->begin() + (indices.at(i) - i));
	}
	*/
}

float PlayingState::DistanceToHero(Pawn * pawn)
{
	float x = _hero->WorldX - pawn->WorldX;
	float y = _hero->WorldY - pawn->WorldY;

	return sqrtf( (x*x) + (y*y) );
}

const float PlayingState::BUBBLE_SIZE = 150.0f;

void PlayingState::Render(sf::RenderWindow * window) {

	//set the camera
	window->setView(*_heroController->GetView());

	//cycle through and render all the walls of the maze	
	for (auto iter = _maze->begin(); iter != _maze->end(); iter++)
	{
		float dist = DistanceToHero((*iter).get());
		float inter = 1.0f - (dist / BUBBLE_SIZE);
		
		if (inter < 0) continue;

		(*iter)->Colour.r *= inter;
		(*iter)->Colour.g *= inter;
		(*iter)->Colour.b *= inter;

		(*iter)->Render(window);

		(*iter)->Colour = (*iter)->RenderColour;
	}

	_hero->Render(window);

	/*
	for (auto iter = _terrors->begin(); iter != _terrors->end(); iter++)
	{
		(*iter)->Render(window);
	}
	*/
}

void PlayingState::ProcessInput(float delta) {
	bool leftBlocked = false;
	bool rightBlocked = false;
	bool topBlocked = false;
	bool bottomBlocked = false;

	auto blocked = _physics->IsColliding(_hero.get());
	_heroController->Process(blocked, delta);

	//_terrorsController->Think();
}

GameState * PlayingState::Update()
{
	if (_physics->AreColliding(_hero.get(), _finishPoint))
	{
		_physics->Reset();
		auto nextLevel = new PlayingState();
		auto options = PlayingStateOptions(_size + 1);
		nextLevel->Init(options);
		delete this;
		return nextLevel;
	}

	return this;
}