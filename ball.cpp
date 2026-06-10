#include "Ball.h"
#include "Constants.h"

Ball::Ball(float x, float y) {
	shape.setRadius(BALL_RADIUS);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);

	stuck = false;
	alive = true;

	velocity = { BALL_SPEED, -BALL_SPEED };
}

void Ball::update() {
	shape.move(velocity);
}


