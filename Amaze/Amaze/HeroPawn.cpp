#include "HeroPawn.h"
#include "GameProperties.h"

HeroPawn::HeroPawn() 
{

//if not debug use the texture, this is to address the issue #18 : https://github.com/CarbonBasedDudeform/Amaze/issues/18
#ifdef NDEBUG
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/hero.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
#endif

#ifdef _DEBUG 
	Size = ((float)GameProperties::SCREEN_WIDTH / (float)GameProperties::SCREEN_HEIGHT) * 10;
	_offset = Size;
#endif
	WorldX = -_offset;
	WorldY = -_offset;
	TakesDamage = true;
	Health = 0;
}


HeroPawn::~HeroPawn()
{
}

void HeroPawn::Render(sf::RenderWindow * window)
{
#ifdef NDEBUG
	_sprite->setPosition(WorldX, WorldY);
	window->draw(*_sprite);
#endif

#if _DEBUG
	sf::CircleShape circle(Size);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(circle);
#endif
}

void HeroPawn::SetRotation(float angle)
{
#ifdef NDEBUG
	_sprite->setRotation(angle);
#endif
}

sf::Vector2f HeroPawn::GetPosition()
{
	auto rot = _sprite->getRotation();
	return sf::Vector2f(WorldX - _offset*cos(rot), WorldY - _offset * sin(rot));
}
