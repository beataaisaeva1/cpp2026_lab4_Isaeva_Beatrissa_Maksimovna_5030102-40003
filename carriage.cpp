#include "carriage.h"

Carriage::Carriage() {
	shape.setSize({ 120.f, 20.f });
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(340.f, 550.f);
}

void Carriage::update() {
    float speed = 7.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shape.move(-speed, 0.f);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shape.move(speed, 0.f);
    }
}



