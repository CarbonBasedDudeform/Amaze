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
	void MoveLeft(float timeDelta);
	void MoveRight(float timeDelta);
	void MoveUp(float timeDelta);
	void MoveDown(float timeDelta);
	void Process(BlockedDirections, float timeDelta) override;
private:
	sf::View * _view;
	HeroPawn * _pawn;
	void UpdateView();
};

