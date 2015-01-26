#pragma once
#include "Pawn.h"
#include <SFML\Graphics.hpp>

struct GridLocation {
	int X;
	int Y;
};

class GridBlock : public Pawn
{
public:
	static const int WALL_LENGTH = 50;

	GridBlock(int,int,int,int);
	~GridBlock();
	int X;
	int Y;

	void Render(sf::RenderWindow *);

	void Enable(bool);
	bool IsEnabled() const;

	bool IsStart() const;
	bool IsFinish() const;
	void MakeStart();
	void MakeFinish();

private:
	int _xOffset;
	int _yOffset;

	sf::RectangleShape * _block;

	bool _display;

	bool _IsStart;
	bool _IsFinish;
};

