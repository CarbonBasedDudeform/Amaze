#pragma once
#include "Pawn.h"
class AIPawn :
	public Pawn
{
public:
	AIPawn();
	~AIPawn();
	void Render(sf::RenderWindow *);
};

