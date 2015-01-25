#pragma once
//for access to sf::vector types
#include "SFML\Graphics.hpp"

//Could create a wrapper class and overload some operators to make the code all nice like
//but so far vectors math is only used in a small part, not worth the time and effort.

class Vector
{
public:
	static sf::Vector2f& Divide(sf::Vector2f rhs, float scalar) {
		auto temp = new sf::Vector2f();
		temp->x = rhs.x / scalar;
		temp->y = rhs.y / scalar;
		return *temp;
	}

	static float Dot(sf::Vector2f one, sf::Vector2f two)
	{
		return ((one.x*two.x) + (one.y*two.y));
	}

	/*
	static sf::Vector2f& Cross(sf::Vector2f one, sf::Vector2f two)
	{
		auto temp = new sf::Vector2f();
		temp.x = 
	}
	*/
};

