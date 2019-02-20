#pragma once
#include "Controller.h"
#include "GameProperties.h"
#include "HeroPawn.h"
#include "Laser.h"
#include "PhysicsSystem.h"

#include "SFML\Graphics.hpp"

/*
	Controls the hero pawn,
	doesn't do much other than stop the hero from walking through walls
*/

class HeroController :
	public Controller
{
public:
	HeroController(HeroPawn*, PhysicsSystem * physics);
	~HeroController();
	sf::View * GetView();
	void MoveLeft(float timeDelta);
	void MoveRight(float timeDelta);
	void MoveUp(float timeDelta);
	void MoveDown(float timeDelta);
	void Process(BlockedDirections, float timeDelta) override;
	std::vector<std::unique_ptr<Laser>> Lasers;
private:
	std::unique_ptr<sf::View> _view;
	HeroPawn * _pawn;
	void UpdateView();
	float _rotation;
	sf::Vector2f _direction;
	PhysicsSystem * _physics;
	float accumulatedTime;
};

