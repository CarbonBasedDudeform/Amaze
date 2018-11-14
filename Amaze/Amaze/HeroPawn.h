#pragma once
#include "Pawn.h"

/*
	The hero of the story that doesn't need to be saved
*/

class HeroPawn :
	public Pawn
{
public:
	HeroPawn();
	~HeroPawn();
	void Render(sf::RenderWindow *);
	void SetRotation(float angle);
};

