#include "AIPawn.h"
#include "GameProperties.h"

AIPawn::AIPawn()
{
	WorldX = 0;
	WorldY = 0;
	TakesDamage = true;
	Health = 100;
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/terror.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
}


AIPawn::~AIPawn()
{
}

void AIPawn::Render(sf::RenderWindow * window)
{
	_sprite->setPosition(WorldX, WorldY);
	window->draw(*_sprite);
}

void AIPawn::SetRotation(float angle)
{
	_sprite->setRotation(angle);
}

float AIPawn::GetRotation() const
{
	return _sprite->getRotation();
}


sf::Vector2f AIPawn::GetPosition()
{
	auto rot = _sprite->getRotation();
	return sf::Vector2f(WorldX - _offset * cos(rot), WorldY - _offset * sin(rot));
}
