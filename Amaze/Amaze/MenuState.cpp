#pragma once
#include "MenuState.h"


MenuState::MenuState()
{
	_nextState = this;
#ifdef NDEBUG
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/menu_bg.png");
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_textureToStart = std::make_shared<sf::Texture>();
	_textureToStart->loadFromFile("Textures/press_to_start.png");
	_spriteToStart = std::make_unique<sf::Sprite>(*_textureToStart);
	_spriteToStart->setPosition(100, 400);
#endif
}


MenuState::~MenuState()
{
}

void MenuState::Init(GameStateOptions opts) {
	//Init code
}

void MenuState::Render(sf::RenderWindow * window) {
#ifdef NDEBUG
	window->draw(*_sprite);
	window->draw(*_spriteToStart);
#endif
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