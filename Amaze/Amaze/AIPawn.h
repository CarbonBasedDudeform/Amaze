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
	void SetRotation(float angle);
	float GetRotation() const;
	sf::Vector2f GetPosition();
	sf::Vector2f Direction;
};

