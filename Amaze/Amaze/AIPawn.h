#pragma once
#include "Pawn.h"

/*
	AI Pawns will mindlessly wander around trying to imitate real life drunk people at the fair.
*/

class AIPawn :
	public Pawn
{
public:
	AIPawn();
	~AIPawn();
	void Render(sf::RenderWindow *);
};

