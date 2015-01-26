#include "HeroPawn.h"


HeroPawn::HeroPawn() 
{
	WorldX = 0;
	WorldY = 0;
//if not debug use the texture, this is to address the issue #18 : https://github.com/CarbonBasedDudeform/Amaze/issues/18
#ifdef NDEBUG
	_texture = new sf::Texture();
	_texture->loadFromFile("Textures/hero.png");
	Size = _texture->getSize().x;
	_offset = Size / 2;
	_sprite = new sf::Sprite(*_texture);
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
	sf::CircleShape circle(10.0f);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(circle);
#endif
}