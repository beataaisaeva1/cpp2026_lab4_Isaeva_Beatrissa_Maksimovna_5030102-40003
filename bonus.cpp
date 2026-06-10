#include "Bonus.h"
#include "Constants.h"

Bonus::Bonus(float x, float y) {
	shape.setRadius(BALL_RADIUS);
	shape.setPosition(x, y);

	active = true;

	shape.setFillColor(sf::Color::Cyan);
}

void Bonus::update() {
	shape.move(0.f, BONUS_SPEED);
}

