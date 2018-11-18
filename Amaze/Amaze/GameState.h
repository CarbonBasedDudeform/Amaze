#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


struct GameStateOptions {};
class StateManager;
/*
	Parent Class which all GameStates inherit
*/
class GameState
{
public:
	GameState();
	//effective C++, ensures that if the derived classes get deleted then the base class destructor is called
	virtual ~GameState();
	virtual void Init(GameStateOptions opts) {}
	virtual void Render(sf::RenderWindow * window) = 0;
	virtual void ProcessInput(float delta) = 0;
	virtual void Update(StateManager& stateMgr) = 0;
};