#pragma once
#include "MenuState.h"
#include "StateManager.h"
#include <sstream>
MenuState::MenuState()
	: _view(sf::Vector2f(GameProperties::SCREEN_WIDTH / 2, GameProperties::SCREEN_HEIGHT / 2), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT)),
	counter(0)
{
	//_texture = std::make_shared<sf::Texture>();
	for (int i = 0; i < 300; i++) {
		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
		std::stringstream filename;
		if (i < 10) {
			filename << "Textures/Menu/background/background00" << i << ".png";
		}
		else if (i < 100)
		{
			filename << "Textures/Menu/background/background0" << i << ".png";
		}
		else
		{
			filename << "Textures/Menu/background/background" << i << ".png";
		}

		texture->loadFromFile(filename.str());
		_texture.push_back(std::move(texture));
	}
	_sprite = std::make_unique<sf::Sprite>(**_texture.begin());
	const auto size = (*_texture.begin())->getSize();
	sf::Vector2f scale(GameProperties::SCREEN_WIDTH / (float)size.x, GameProperties::SCREEN_HEIGHT / (float)size.y);
	_sprite->setScale(scale);

	_textureToStart = std::make_shared<sf::Texture>();
	_textureToStart->loadFromFile("Textures/press_to_start.png");
	_spriteToStart = std::make_unique<sf::Sprite>(*_textureToStart);
	const auto startSize = _textureToStart->getSize();
	_spriteToStart->setScale(scale);
	_spriteToStart->setPosition((GameProperties::SCREEN_WIDTH - (startSize.x*scale.x))/2.0f, (GameProperties::SCREEN_HEIGHT - (startSize.y*scale.y)) / 1.2f);
}


MenuState::~MenuState()
{
}

void MenuState::Init(GameStateOptions opts) {
	//Init code
}
	
void MenuState::Render(sf::RenderWindow * window) {
	window->setView(_view);
	window->draw(*_sprite);
	window->draw(*_spriteToStart);
	counter = (counter + 1) % _texture.size();
	_sprite->setTexture(*_texture.at(counter));

}

void MenuState::ProcessInput(float delta) {

}

void MenuState::Update(StateManager& stateMgr) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		stateMgr.ChangeState(StateManager::Playing);
	}
}