#include "GridBlock.h"

const int GridBlock::WALL_WIDTH;
const int GridBlock::WALL_LENGTH;

GridBlock::GridBlock(int x, int y, int xOffset, int yOffset)
{
	_xOffset = xOffset;
	_yOffset = yOffset;
	X = x;
	Y = y;
	//left wall stuff
	_leftWall = new sf::RectangleShape(sf::Vector2f(GridBlock::WALL_WIDTH, GridBlock::WALL_LENGTH));
	_leftWall->setFillColor(sf::Color::Red);
	_leftWall->setPosition(sf::Vector2f(xOffset + X - (GridBlock::WALL_LENGTH / 2), yOffset + Y));
	_displayLeft = true;
	//right wall stuff
	_rightWall = new sf::RectangleShape(sf::Vector2f(GridBlock::WALL_WIDTH, GridBlock::WALL_LENGTH));
	_rightWall->setFillColor(sf::Color::Red);
	_rightWall->setPosition(sf::Vector2f(xOffset + X + (GridBlock::WALL_LENGTH / 2), yOffset + Y));
	_displayRight = true;
	//top wall stuff
	_topWall = new sf::RectangleShape(sf::Vector2f(GridBlock::WALL_LENGTH, GridBlock::WALL_WIDTH));
	_topWall->setFillColor(sf::Color::Red);
	_topWall->setPosition(sf::Vector2f(xOffset + X - (GridBlock::WALL_LENGTH / 2),yOffset + Y + (GridBlock::WALL_LENGTH - GridBlock::WALL_WIDTH)));
	_displayTop = true;
	//bottom wall stuff
	_bottomWall = new sf::RectangleShape(sf::Vector2f(GridBlock::WALL_LENGTH, GridBlock::WALL_WIDTH));
	_bottomWall->setFillColor(sf::Color::Red);
	_bottomWall->setPosition(sf::Vector2f(xOffset + X - (GridBlock::WALL_LENGTH / 2),yOffset + Y ));
	_displayBottom = true;

	//default to neither start nor finish
	_IsStart = false;
	_IsFinish = false;
}


GridBlock::~GridBlock()
{
	delete _leftWall;
	delete _rightWall;
	delete _topWall;
	delete _bottomWall;
}

void GridBlock::Render(sf::RenderWindow * window)
{
	if (_displayLeft) window->draw(*_leftWall);
	if (_displayRight) window->draw(*_rightWall);
	if (_displayTop) window->draw(*_topWall);
	if (_displayBottom) window->draw(*_bottomWall);
}

void GridBlock::EnableLeft(bool val)
{
	_displayLeft = val;
}

void GridBlock::EnableRight(bool val)
{
	_displayRight = val;
}

void GridBlock::EnableTop(bool val){
	_displayTop = val;
}

void GridBlock::EnableBottom(bool val)
{
	_displayBottom = val;
}

bool GridBlock::IsStart() const {
	return _IsStart;
}

bool GridBlock::IsFinish() const {
	return _IsFinish;
}

void GridBlock::MakeStart() {
	_IsStart = true;
}

void GridBlock::MakeFinish() {
	_IsFinish = true;
}