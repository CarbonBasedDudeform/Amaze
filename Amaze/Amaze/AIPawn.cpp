#include "AIPawn.h"
#include "GameProperties.h"

AIPawn::AIPawn()
{
	WorldX = 0;
	WorldY = 0;
	//if not debug use the texture, this is to address the issue #18 : https://github.com/CarbonBasedDudeform/Amaze/issues/18
#ifdef NDEBUG
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/terror.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
#endif

#ifdef _DEBUG 
	Size = ((float)GameProperties::SCREEN_WIDTH / (float)GameProperties::SCREEN_HEIGHT) * 10;
#endif
}


AIPawn::~AIPawn()
{
}

void AIPawn::Render(sf::RenderWindow * window)
{
#ifdef NDEBUG
	_sprite->setPosition(WorldX, WorldY);
	window->draw(*_sprite);
#endif

#if _DEBUG
	sf::CircleShape circle(Size);
	circle.setFillColor(sf::Color::Blue);
	circle.setPosition(WorldX - _offset, WorldY - _offset);
	window->draw(circle);
#endif
}

void AIPawn::SetRotation(float angle)
{
	_sprite->setRotation(angle);
}
