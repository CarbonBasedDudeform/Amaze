#include "HeroPawn.h"
#include "GameProperties.h"

HeroPawn::HeroPawn() 
{
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/hero.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
	WorldX = -_offset;
	WorldY = -_offset;
	TakesDamage = true;
	Health = 100;
}


HeroPawn::~HeroPawn()
{
}

void HeroPawn::Render(sf::RenderWindow * window)
{
	_sprite->setPosition(WorldX, WorldY);
	window->draw(*_sprite);
}

void HeroPawn::SetRotation(float angle)
{
	_sprite->setRotation(angle);
}

sf::Vector2f HeroPawn::GetPosition()
{
	auto rot = _sprite->getRotation();
	return sf::Vector2f(WorldX - _offset*cos(rot), WorldY - _offset * sin(rot));
}
