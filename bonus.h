#pragma once
#include <SFML/Graphics.hpp>

enum BonusType {
	BIG_CARRIAGE,
	SLOW_BALL,
	STICKY_BALL,
	BOTTOM_SHIELD,
	EXTRA_BALL
};

class Bonus {
public:
	sf::CircleShape shape;

	BonusType type;
	bool active;

	Bonus(float x, float y, BonusType t);

	void update();
};


