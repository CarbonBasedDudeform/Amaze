#pragma once
#include "MenuState.h"


MenuState::MenuState()
{
	_nextState = this;
}


MenuState::~MenuState()
{
}

void MenuState::Init(GameStateOptions opts) {
	//Init code
}

void MenuState::Render(sf::RenderWindow * window) {
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	window->draw(shape);
}

void MenuState::ProcessInput(float delta) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		auto options = PlayingStateOptions(INITIAL_MAZE_SIZE, 0);
		auto state = new PlayingState();
		state->Init(options);
		_nextState = state;
	}
}

GameState * MenuState::Update() {
	return _nextState;
}