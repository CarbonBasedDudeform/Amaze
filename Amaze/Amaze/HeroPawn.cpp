#include "HeroPawn.h"
#include "GameProperties.h"

HeroPawn::HeroPawn() 
{
	WorldX = 0;
	WorldY = 0;
//if not debug use the texture, this is to address the issue #18 : https://github.com/CarbonBasedDudeform/Amaze/issues/18
#ifdef NDEBUG
	_texture = std::make_unique<sf::Texture>();
	_texture->loadFromFile("Textures/hero.png");
	Size = _texture->getSize().x;
	_offset = Size / 2;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
#endif

#ifdef _DEBUG 
	Size = ((float)GameProperties::SCREEN_WIDTH / (float)GameProperties::SCREEN_HEIGHT) * 10;
#endif
}


HeroPawn::~HeroPawn()
{
}

void HeroPawn::Render(sf::RenderWindow * window)
{
#ifdef NDEBUG
	_sprite->setPosition(WorldX-_offset, WorldY-_offset);
	window->draw(*_sprite);
#endif

#if _DEBUG
	sf::CircleShape circle(Size);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(circle);
#endif
}