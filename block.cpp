#include "Block.h"
#include "Constants.h"

Block::Block(float x, float y, BlockType t, int health) {
	shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
	shape.setPosition(x, y);

	hp = health;
	alive = true;
	type = t;

	switch (type) {
	case UNBREAKABLE:
		shape.setFillColor(sf::Color::Blue);
		break;

	case BONUS:
		shape.setFillColor(sf::Color::Yellow);
		break;

	case SPEED:
		shape.setFillColor(sf::Color::Red);
		break;

	case HEALTH:
		shape.setFillColor(sf::Color::Magenta);
		break;
	}
}


