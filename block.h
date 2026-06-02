#pragma once
#include <SFML/Graphics.hpp>

enum BlockType {
	UNBREAKABLE,
	BONUS,
	SPEED,
	HEALTH
};

class Block {
public:
	sf::RectangleShape shape;

	int hp;
	bool alive;
	BlockType type;

	Block(float x, float y, BlockType t, int health = 1);
};

