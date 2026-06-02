#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;

	Ball(float x, float y);

	void update();
};

