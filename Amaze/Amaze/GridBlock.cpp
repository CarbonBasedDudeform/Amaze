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
	IsBlocking = true;
	X = x;
	Y = y;
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/brick.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
}

void GridBlock::Render(sf::RenderWindow * window)
{
		_sprite->setPosition(WorldX, WorldY);
		window->draw(*_sprite);
}

void GridBlock::Enable(bool val)
{
	_display = val;

	if (!val && !_IsFinish) 
	{
		_texture->loadFromFile("Textures/floor.png");
		_sprite->setTexture(*_texture);
	}

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
	_texture->loadFromFile("Textures/floor.png");
	_sprite->setTexture(*_texture);
}

void GridBlock::MakeFinish() {
	_IsFinish = true;
	_texture->loadFromFile("Textures/finish.png");
	_sprite->setTexture(*_texture);
}



void GridBlock::IsCollidable() {

}