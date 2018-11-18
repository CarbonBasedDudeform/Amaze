#pragma once
#include "MenuState.h"
#include "StateManager.h"

MenuState::MenuState()
	: _view(sf::Vector2f(GameProperties::SCREEN_WIDTH / 2, GameProperties::SCREEN_HEIGHT / 2), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT))
{
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
	window->setView(_view);
	window->draw(*_sprite);
	window->draw(*_spriteToStart);
#endif
}

void MenuState::ProcessInput(float delta) {

}

void MenuState::Update(StateManager& stateMgr) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		stateMgr.ChangeState(StateManager::Playing);
	}
}