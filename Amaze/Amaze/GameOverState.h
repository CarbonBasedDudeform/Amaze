#pragma once
#include "GameState.h"

/*
	Class that will manage the game over scene.
*/

class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();
	void Init(GameStateOptions opts) override;
	void Render(sf::RenderWindow * window) override;
	void ProcessInput(float delta) override;
	void Update(StateManager& stateMgr) override;
	void SetScore(int score);
private:
	sf::View _view;
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	sf::Text score;
	sf::Text spaceToContinue;
	sf::Font font;
};

