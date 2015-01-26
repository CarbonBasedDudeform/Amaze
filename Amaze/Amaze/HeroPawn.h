#pragma once
#include "Pawn.h"

class HeroPawn :
	public Pawn
{
public:
	HeroPawn();
	~HeroPawn();
	void Render(sf::RenderWindow *);
};

