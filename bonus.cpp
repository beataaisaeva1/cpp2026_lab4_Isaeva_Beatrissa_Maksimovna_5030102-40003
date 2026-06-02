#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType t) {
	shape.setRadius(10.f);
	shape.setPosition(x, y);

	active = true;
	type = t;

	shape.setFillColor(sf::Color::Cyan);
}

void Bonus::update() {
	shape.move(0.f, 3.f);
}

