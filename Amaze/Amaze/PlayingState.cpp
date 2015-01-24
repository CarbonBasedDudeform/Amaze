#include "PlayingState.h"


PlayingState::PlayingState()
{
}


PlayingState::~PlayingState()
{
}

void PlayingState::Init() {
	//Init code
	_walls = GenerateMaze(10);
}

//size is used to generate a maze of size by size dimension
std::list<sf::RectangleShape *> * PlayingState::GenerateMaze(int size) {
	//some settings like length of a wall, width
	const int WALL_LENGTH = 50;
	const int WALL_WIDTH = 10;
	const int SPACING = 50;

	//our return list containing all the walls
	auto walls = new std::list<sf::RectangleShape *>();
	//populate
	sf::RectangleShape * temp;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			temp = new sf::RectangleShape(sf::Vector2f(WALL_WIDTH, WALL_LENGTH));
			temp->setFillColor(sf::Color::Red);
			temp->setPosition(sf::Vector2f(((i*SPACING) + WALL_WIDTH), (j*SPACING)+WALL_LENGTH));

			walls->push_back(temp);
		}
	}

	//minus one so it doesn't overshoot on the right hand side
	for (int i = 0; i < (size-1); ++i) {
		for (int j = 0; j <= size; ++j) { //<= so that the bottom row is 'sealed'
			temp = new sf::RectangleShape(sf::Vector2f(WALL_LENGTH, WALL_WIDTH));
			temp->setFillColor(sf::Color::Red);
			temp->setPosition(sf::Vector2f(((i*SPACING) + WALL_WIDTH), (j*SPACING) + WALL_LENGTH));

			walls->push_back(temp);
		}
	}
	return walls;
}

void PlayingState::Render(sf::RenderWindow * window) {
	//cycle through and render all the walls of the maze	
	for (auto iter = _walls->begin(); iter != _walls->end(); iter++)
	{
		window->draw(*(iter._Ptr->_Myval));
	}
}