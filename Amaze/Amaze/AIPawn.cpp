#include "AIPawn.h"


AIPawn::AIPawn()
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

#ifdef _DEBUG 
	Size = 10;
#endif
}


AIPawn::~AIPawn()
{
}

void AIPawn::Render(sf::RenderWindow * window)
{
#ifdef NDEBUG
	_sprite->setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(*_sprite);
#endif

#if _DEBUG
	sf::CircleShape circle(Size);
	circle.setFillColor(sf::Color::Blue);
	circle.setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(circle);
#endif
}
