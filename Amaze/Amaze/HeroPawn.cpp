#include "HeroPawn.h"
#include "GameProperties.h"

HeroPawn::HeroPawn() 
{
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/hero.png");
	sf::Vector2f scale(GameProperties::SCREEN_WIDTH / 800.0f, GameProperties::SCREEN_HEIGHT / (float)600.0f);
	Size = _texture->getSize().x;// *scale.x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
	_sprite->setScale(scale);
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
