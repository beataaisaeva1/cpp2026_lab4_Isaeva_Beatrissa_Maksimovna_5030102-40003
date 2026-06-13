#include "Carriage.h"
#include "Constants.h"

Carriage::Carriage() {
    shape.setSize({CARRIAGE_WIDTH, CARRIAGE_HEIGHT });
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(340.f, 550.f);
}

void Carriage::update() {
    float speed = CARRIAGE_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shape.move(-speed, 0.f);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shape.move(speed, 0.f);
    }
}

