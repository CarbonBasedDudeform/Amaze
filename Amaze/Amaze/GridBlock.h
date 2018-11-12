#pragma once
#include "Pawn.h"
#include <SFML\Graphics.hpp>
#include <memory>
#include "GameProperties.h"

struct GridLocation {
	int X;
	int Y;
};

/*
	One of the blocks that make up the maze
*/

class GridBlock : public Pawn
{
public:
	static const int WALL_LENGTH = ((float)GameProperties::SCREEN_WIDTH / (float)GameProperties::SCREEN_HEIGHT) *50;

	GridBlock(int,int,int,int);
	int X;
	int Y;

	void Render(sf::RenderWindow *);

	void Enable(bool);
	bool IsEnabled() const;

	bool IsStart() const;
	bool IsFinish() const;
	void MakeStart();
	void MakeFinish();
	void IsCollidable();

private:
	int _xOffset;
	int _yOffset;

	std::unique_ptr<sf::RectangleShape> _block;

	bool _display;

	bool _IsStart;
	bool _IsFinish;
};

