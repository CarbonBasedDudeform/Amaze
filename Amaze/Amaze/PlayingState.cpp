#include "PlayingState.h"
#include "StateManager.h"

PlayingState::PlayingState()
{
	//seed rand to make everything more unique for each playingstate
	srand((unsigned int)time(NULL));
	_hero = std::make_unique<HeroPawn>();
	_physics = std::make_unique<PhysicsSystem>();
	_maze = std::make_unique<Maze>(_physics.get());
	_terrors = std::make_unique<std::vector<std::unique_ptr<AIPawn>>>();
	healthOutlineTexture.loadFromFile("Textures/healthbar_outline.png");
	healthOutline.setTexture(healthOutlineTexture);
	healthTexture.loadFromFile("Textures/healthbar.png");
	health.setTexture(healthTexture);
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
		if ((*iter)->Health > 0)
			(*iter)->Render(window);
	}

	window->setView(window->getDefaultView());	
	
	window->draw(healthOutline);
	window->draw(health);
}

void PlayingState::ProcessInput(float delta) {
	auto blocked = _physics->IsColliding(_hero.get());
	_heroController->Process(blocked, delta);
	_terrorsController->Process(blocked, delta);
	float healthScale = _hero->Health > 10 ?_hero->Health / 100.0f : 0;
	health.setScale(healthScale, 1.0f);
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

void PlayingState::Update(StateManager& stateMgr)
{
	if (_physics->AreColliding(_hero.get(), _maze->GetFinish()))
	{
		stateMgr.ChangeState(StateManager::Playing);
	} 
	else if (_hero->Health < 0) 
	{
		stateMgr.ChangeState(StateManager::GameOver);
	}
}