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

	X = x;
	Y = y;
#ifdef _DEBUG
	Size = WALL_LENGTH;

	_offset = Size / 2.0f;
	
	_block = std::make_unique<sf::RectangleShape>(sf::Vector2f(GridBlock::WALL_LENGTH, GridBlock::WALL_LENGTH));
	_block->setFillColor(sf::Color::Red);
	_block->setPosition(sf::Vector2f(WorldX - _offset, WorldY - _offset));
#endif
#ifdef NDEBUG
	_texture = std::make_shared<sf::Texture>();
	_texture->loadFromFile("Textures/brick.png");
	Size = _texture->getSize().x;
	_offset = Size / 2.0f;
	_sprite = std::make_unique<sf::Sprite>(*_texture);
	_sprite->setOrigin(_offset, _offset);
#endif
}

void GridBlock::Render(sf::RenderWindow * window)
{
#ifdef NDEBUG
		_sprite->setPosition(WorldX, WorldY);
		window->draw(*_sprite);
#endif
#ifdef _DEBUG
		_block->setFillColor(Colour);
		window->draw(*_block);
#endif
}

void GridBlock::Enable(bool val)
{
	_display = val;

	if (!val && !_IsFinish) 
	{
#ifdef NDEBUG
		_texture->loadFromFile("Textures/floor.png");
		_sprite->setTexture(*_texture);
#endif
#ifdef _DEBUG
		Colour = sf::Color::Black;
		RenderColour = Colour;

#endif
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
#ifdef _DEBUG
	Colour = sf::Color::Yellow;
	RenderColour = Colour;
#endif

#ifdef NDEBUG
	_texture->loadFromFile("Textures/floor.png");
	_sprite->setTexture(*_texture);
#endif
}

void GridBlock::MakeFinish() {
	_IsFinish = true;
#ifdef _DEBUG
	Colour = sf::Color::Magenta;
	RenderColour = Colour;
#endif

#ifdef NDEBUG
	_texture->loadFromFile("Textures/finish.png");
	_sprite->setTexture(*_texture);

#endif
}



void GridBlock::IsCollidable() {
#ifdef _DEBUG
	_block->setFillColor(sf::Color::White);
	Colour = sf::Color::White;
	RenderColour = sf::Color::White;
#endif
}