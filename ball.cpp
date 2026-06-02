#include "ball.h"

Ball::Ball(float x, float y) {
	shape.setRadius(10.f);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);

	velocity = { 3.f, -3.f };
}

void Ball::update() {
	shape.move(velocity);
}

