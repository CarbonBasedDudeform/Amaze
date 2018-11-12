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

/*
	This class populates the world with Pawns, it places the hero (the player) at the start
	and creates the number of terrors (ai enemies) to be placed in the maze.
	In a vague sense, it manages the numbers for the level.
	It passes on calls to process input for active pawns in the level
	and render calls for them.
	In a dumb sense, it manages items in the world when in the game is being played.
*/

struct PlayingStateOptions {
	int LevelSize;
	int TerrorPopulation;
	PlayingStateOptions(int size, int terrors) : LevelSize(size), TerrorPopulation(terrors) {};
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
	std::unique_ptr<std::vector<std::unique_ptr<AIPawn>>> _terrors;
	std::unique_ptr<AIController> _terrorsController;

	static const float BUBBLE_SIZE;
};

