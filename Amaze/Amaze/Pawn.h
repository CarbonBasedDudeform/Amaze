#pragma once
#include "SFML\Graphics.hpp"

/*
Pawns represent contrallable Entities
such as the terrors (the ai) and the hero (the player)
*/

class Pawn
{
public:
	Pawn();
	virtual ~Pawn();
	float WorldX;
	float WorldY;
	virtual void Render(sf::RenderWindow * window) = 0;
	float Size;
protected:
	sf::Texture * _texture;
	sf::Sprite * _sprite;
	float _offset;
};
