#pragma once
#include "Controller.h"
#include "GameProperties.h"
#include "HeroPawn.h"

#include "SFML\Graphics.hpp"

class HeroController :
	public Controller
{
public:
	HeroController(HeroPawn*);
	~HeroController();
	sf::View * GetView();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
private:
	sf::View * _view;
	HeroPawn * _pawn;
	void UpdateView();
};

