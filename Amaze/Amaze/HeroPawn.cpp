#include "HeroPawn.h"


HeroPawn::HeroPawn() 
{
	WorldX = 0;
	WorldY = 0;

	_texture = new sf::Texture();
	_texture->loadFromFile("Textures/hero.png");
	Size = _texture->getSize().x;
	_offset = Size / 2;
	_sprite = new sf::Sprite(*_texture);
}


HeroPawn::~HeroPawn()
{
}

void HeroPawn::Render(sf::RenderWindow * window)
{
	_sprite->setPosition(WorldX-_offset, WorldY-_offset);
	window->draw(*_sprite);
}