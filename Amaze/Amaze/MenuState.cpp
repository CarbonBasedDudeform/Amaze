#include "MenuState.h"


MenuState::MenuState()
{
}


MenuState::~MenuState()
{
}

void MenuState::Init() {
	//Init code
}

void MenuState::Render(sf::RenderWindow * window) {
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	window->draw(shape);
}