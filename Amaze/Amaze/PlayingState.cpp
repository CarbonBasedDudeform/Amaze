#include "PlayingState.h"


PlayingState::PlayingState()
{
}


PlayingState::~PlayingState()
{
}

void PlayingState::Init() {
	//Init code
}

void PlayingState::Render(sf::RenderWindow * window) {
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Red);
	window->draw(shape);
}