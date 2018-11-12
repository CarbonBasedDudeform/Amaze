#pragma once
#include "SFML\Graphics.hpp"
#include <memory>

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
	sf::Color Colour;
	sf::Color RenderColour;
protected:
	std::unique_ptr<sf::Texture> _texture;
	std::unique_ptr<sf::Sprite> _sprite;
	float _offset;
};

