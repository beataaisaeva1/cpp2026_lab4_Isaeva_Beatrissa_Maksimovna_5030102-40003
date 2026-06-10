#include "SlowBallBonus.h"

#include "Game.h"

SlowBallBonus::SlowBallBonus(float x, float y) : Bonus(x, y) {
}

void SlowBallBonus::activate(Game& game) {
    game.slowBalls();
}

