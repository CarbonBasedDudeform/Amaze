#include "GridBlock.h"

const int GridBlock::WALL_LENGTH;

GridBlock::GridBlock(int x, int y, int xOffset, int yOffset)
	: _IsStart(false),
	_IsFinish(false),
	_display(true),
	_xOffset(xOffset),
	_yOffset(yOffset)
{
	//Pawn Props
	WorldX = xOffset + WALL_LENGTH / 2.0f;
	WorldY = yOffset + WALL_LENGTH / 2.0f;
	Size = WALL_LENGTH;

	X = x;
	Y = y;
	_offset = Size / 2.0f;
	//left wall stuff
	_block = std::make_unique<sf::RectangleShape>(sf::Vector2f(GridBlock::WALL_LENGTH, GridBlock::WALL_LENGTH));
	_block->setFillColor(sf::Color::Red);
	_block->setPosition(sf::Vector2f(WorldX - _offset, WorldY - _offset));
}

void GridBlock::Render(sf::RenderWindow * window)
{
	if (_display || _IsFinish || _IsStart) 	{
		_block->setFillColor(Colour);
		window->draw(*_block);
	}
}

void GridBlock::Enable(bool val)
{
	_display = val;
}

bool GridBlock::IsEnabled() const {
	return _display;
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

	Colour = sf::Color::Yellow;
	RenderColour = Colour;
}

void GridBlock::MakeFinish() {
	_IsFinish = true;
	_display = false;

	Colour = sf::Color::Magenta;
	RenderColour = Colour;
}



void GridBlock::IsCollidable() {
	_block->setFillColor(sf::Color::White);
	Colour = sf::Color::White;
	RenderColour = sf::Color::White;
}