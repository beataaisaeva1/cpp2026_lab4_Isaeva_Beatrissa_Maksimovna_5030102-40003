#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Bonus {
public:
	sf::CircleShape shape;

	bool active;

	Bonus(float x, float y);

	virtual void update();

	virtual void activate(Game& game) = 0;

	virtual ~Bonus() = default;
};

