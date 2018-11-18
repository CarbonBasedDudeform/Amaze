#include "PlayingState.h"

PlayingState::PlayingState()
{
	//seed rand to make everything more unique for each playingstate
	srand((unsigned int)time(NULL));
	_hero = std::make_unique<HeroPawn>();
	_physics = std::make_unique<PhysicsSystem>();
	_maze = std::make_unique<Maze>(_physics.get());
	_terrors = std::make_unique<std::vector<std::unique_ptr<AIPawn>>>();
}


PlayingState::~PlayingState()
{
}

void PlayingState::Init(PlayingStateOptions opts) {
	//Init code
	 _maze->GenerateMaze(opts.LevelSize);
	 auto start = _maze->GetStart();
	 _hero->WorldX = start->WorldX;
	 _hero->WorldY = start->WorldY;

	//:::::::::::::::IMPORTANT::::::::::::::: HeroController is created here so that the camera can be focused on the hero pawn correctly.
	//										 aka, herocontroller needs to be created after the pawns WorldX and Y have been set
	_heroController = std::make_unique<HeroController>(_hero.get(), _physics.get());
	for (int i = 0; i < opts.TerrorPopulation; i++) {
		_terrors->push_back(std::make_unique<AIPawn>());
	}
	_terrorsController = std::make_unique<AIController>(_terrors.get(), _maze.get(), _physics.get(), _hero.get());
}

const float PlayingState::BUBBLE_SIZE = 300.0f;

void PlayingState::Render(sf::RenderWindow * window) {

	//set the camera
	window->setView(*_heroController->GetView());
	_maze->Render(window, BUBBLE_SIZE, _hero.get());
	
	for (auto iter = _heroController->Lasers.begin(); iter != _heroController->Lasers.end(); iter++) {
		(*iter)->Render(window);
	}

	for (auto iter = _terrorsController->Lasers.begin(); iter != _terrorsController->Lasers.end(); iter++) {
		(*iter)->Render(window);
	}

	_hero->Render(window);

	
	for (auto iter = _terrors->begin(); iter != _terrors->end(); iter++)
	{
		(*iter)->Render(window);
	}
}

void PlayingState::ProcessInput(float delta) {
	auto blocked = _physics->IsColliding(_hero.get());
	_heroController->Process(blocked, delta);
	_terrorsController->Process(blocked, delta);

	for (auto iter = _heroController->Lasers.begin(); iter != _heroController->Lasers.end();) {
		if ((*iter)->IsDead()) {
			_physics->RemoveCollidable((*iter).get());
			iter = _heroController->Lasers.erase(iter);
		}
		else
		{
			auto blocked = _physics->IsColliding((*iter).get());
			(*iter)->Update(blocked);
			iter++;
		}
	}

	for (auto iter = _terrorsController->Lasers.begin(); iter != _terrorsController->Lasers.end();) {
		if ((*iter)->IsDead()) {
			_physics->RemoveCollidable((*iter).get());
			iter = _terrorsController->Lasers.erase(iter);
		}
		else
		{
			auto blocked = _physics->IsColliding((*iter).get());
			(*iter)->Update(blocked);
			iter++;
		}
	}
}

GameState * PlayingState::Update()
{
	if (_physics->AreColliding(_hero.get(), _maze->GetFinish()))
	{
		_physics->Reset();
		auto nextLevel = new PlayingState();
		auto options = PlayingStateOptions(_maze->GetSize() + 1, _terrors->size() + 1);
		nextLevel->Init(options);
		delete this;
		return nextLevel;
	}

	return this;
}