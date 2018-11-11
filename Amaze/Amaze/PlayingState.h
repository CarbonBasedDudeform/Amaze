#pragma once
#include <SFML\Graphics\Shader.hpp>
#include "GameState.h"
#include "GridBlock.h"

#include "HeroController.h"
#include "HeroPawn.h"

#include "AIController.h"
#include "AIPawn.h"

#include "PhysicsSystem.h"
#include "Maze.h"

//stl
#include <vector>

struct PlayingStateOptions {
	int LevelSize;
	PlayingStateOptions(int size) : LevelSize(size) {};
};

class PlayingState :
	public GameState
{
public:
	PlayingState();
	~PlayingState();
	void Init(PlayingStateOptions opts);
	void Render(sf::RenderWindow * window) override;
	void ProcessInput(float delta) override;
	GameState * Update() override;


private:
	std::unique_ptr<PhysicsSystem> _physics;
	std::unique_ptr<Maze> _maze;
	//The Hero (Thero)
	std::unique_ptr<HeroPawn> _hero;
	std::unique_ptr<HeroController> _heroController;

	//The Terrors (The T errors)
	std::vector<AIPawn *> * _terrors;
	AIController * _terrorsController;
	int _placedAI;

	static const float BUBBLE_SIZE;
};

