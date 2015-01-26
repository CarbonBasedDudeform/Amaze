#pragma once
#include <SFML\Graphics.hpp>

struct GridLocation {
	int X;
	int Y;
};

class GridBlock
{
public:
	static const int WALL_LENGTH = 50;
	static const int WALL_WIDTH = 10;

	GridBlock(int,int,int,int);
	~GridBlock();
	int X;
	int Y;

	void Render(sf::RenderWindow *);

	void EnableLeft(bool);
	void EnableRight(bool);
	void EnableTop(bool);
	void EnableBottom(bool);

	bool IsStart() const;
	bool IsFinish() const;
	void MakeStart();
	void MakeFinish();
	float WorldX;
	float WorldY;

private:
	int _xOffset;
	int _yOffset;

	sf::RectangleShape * _leftWall;
	sf::RectangleShape * _rightWall;
	sf::RectangleShape * _topWall;
	sf::RectangleShape * _bottomWall;

	bool _displayLeft;
	bool _displayRight;
	bool _displayTop;
	bool _displayBottom;

	bool _IsStart;
	bool _IsFinish;

	void NoWalls();
};

