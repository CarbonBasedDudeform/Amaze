#include "HeroPawn.h"


HeroPawn::HeroPawn() 
{
	WorldX = 0;
	WorldY = 0;
}


HeroPawn::~HeroPawn()
{
}

void HeroPawn::Render(sf::RenderWindow * window)
{
	sf::RectangleShape rect(sf::Vector2f(10.0f, 10.0f));
	rect.setFillColor(sf::Color::Green);
	rect.setPosition(WorldX, WorldY);
	window->draw(rect);
}