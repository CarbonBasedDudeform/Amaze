#include "GameOverState.h"
#include "StateManager.h"
#include "GameProperties.h"

GameOverState::GameOverState() :
	_view(sf::Vector2f(GameProperties::SCREEN_WIDTH / 2, GameProperties::SCREEN_HEIGHT / 2), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT))
{
	texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("Textures/death.png");
	sprite = std::make_unique<sf::Sprite>();
	sprite->setTexture(*texture);

	font.loadFromFile("Fonts/RobotoCondensed-Bold.ttf");
	score.setFont(font);
	score.setFillColor(sf::Color::Red);
	score.setPosition(375, 250);
	score.setString("50000");

	spaceToContinue.setFont(font);
	spaceToContinue.setPosition(250, 500);
	spaceToContinue.setString("Press Space to Continue");
}


GameOverState::~GameOverState()
{
}

void GameOverState::Init(GameStateOptions opts) {
	//Init code
}

void GameOverState::Render(sf::RenderWindow * window) {
	//render code
	window->setView(_view);
	window->draw(*sprite);
	window->draw(score);
	window->draw(spaceToContinue);
}

void GameOverState::ProcessInput(float delta) {

}

void GameOverState::Update(StateManager& stateMgr) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		stateMgr.ChangeState(StateManager::Menu);
	}
}

void GameOverState::SetScore(int playerScore) {
	score.setString(std::to_string(playerScore));
}