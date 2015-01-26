#include "GridBlock.h"

const int GridBlock::WALL_LENGTH;

GridBlock::GridBlock(int x, int y, int xOffset, int yOffset)
{
	_xOffset = xOffset;
	_yOffset = yOffset;
	X = x;
	Y = y;

	WorldX = _xOffset + X;
	WorldY = _yOffset + Y + WALL_LENGTH/2;

	//left wall stuff
	_block = new sf::RectangleShape(sf::Vector2f(GridBlock::WALL_LENGTH, GridBlock::WALL_LENGTH));
	_block->setFillColor(sf::Color::Red);
	_block->setPosition(sf::Vector2f(xOffset + X, yOffset + Y));
	_display = true;

	//default to neither start nor finish
	_IsStart = false;
	_IsFinish = false;
}


GridBlock::~GridBlock()
{
	delete _block;
}

void GridBlock::Render(sf::RenderWindow * window)
{
	if (_display) window->draw(*_block);
}

void GridBlock::Enable(bool val)
{
	_display = val;
}

bool GridBlock::IsStart() const {
	return _IsStart;
}

bool GridBlock::IsFinish() const {
	return _IsFinish;
}

void GridBlock::MakeStart() {
	_IsStart = true;
	_display = false;
}

void GridBlock::MakeFinish() {
	_IsFinish = true;
	_display = false;
}