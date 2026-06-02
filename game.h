#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "ball.h"
#include "carriage.h"
#include "block.h"
#include "bonus.h"

class Game {
private:

	std::mt19937 rng;

	sf::RenderWindow window;

	bool gameStarted;

	Carriage carriage;
	std::vector<Ball> balls;
	std::vector<Block> blocks;
	std::vector<Bonus> bonuses;

	int score;
	int defeats;

	bool shieldActive;
	bool stickyActive;

public:
	Game();

	void run();
};