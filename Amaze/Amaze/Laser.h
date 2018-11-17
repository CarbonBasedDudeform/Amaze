#pragma once
#include "SFML\Graphics.hpp"
#include <memory>
#include "Pawn.h"
#include "PhysicsSystem.h"

class Laser : public Pawn
{
public:
	Laser(const sf::Vector2f& origin, const sf::Vector2f& dir, float rotation, const std::string& texture);
	void Render(sf::RenderWindow *);
	void Update(const BlockedDirections& blocked);
	bool IsDead() const;
private:
	sf::Vector2f _dir;
	std::unique_ptr<sf::Texture> _texture;
	std::unique_ptr<sf::Sprite> _sprite;
	sf::Vector2f _energy;
};

