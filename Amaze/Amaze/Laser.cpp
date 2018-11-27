#include "Laser.h"



Laser::Laser(const sf::Vector2f& origin, const sf::Vector2f& dir, float rotation, const std::string& texture) :
	_dir(dir),
	_energy(10.0f, 10.0f)
{
	_texture = std::make_unique<sf::Texture>();
	_texture->loadFromFile(texture);
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setRotation(rotation);

	WorldX = origin.x;
	WorldY = origin.y;
	Size = 1;
	TakesDamage = false;
	DoesDamage = true;
	DamageAmount = 1;
}

void Laser::Render(sf::RenderWindow * window)
{
	_sprite->setPosition(WorldX, WorldY);
	window->draw(*_sprite);
}

void Laser::Update(const BlockedDirections & blocked)
{
	if (blocked.Up.Blocked || blocked.Down.Blocked) {
		_energy.x *= -1 * 0.75f;
		DamageAmount *= 0.75f;
	}
	if (blocked.Left.Blocked || blocked.Right.Blocked) {
		_energy.y *= -1 * 0.75f;
		DamageAmount *= 0.75f;
	}
	WorldX += _dir.x * _energy.x;
	WorldY += _dir.y * _energy.y;
}

bool Laser::IsDead() const
{
	return abs(_energy.x + _energy.y) < 0.2f;
}
