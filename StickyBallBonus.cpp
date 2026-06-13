#include "StickyBallBonus.h"

#include "Game.h"

StickyBallBonus::StickyBallBonus(float x, float y) : Bonus(x, y) {
}

void StickyBallBonus::activate(Game& game) {
    game.activateStickyBall();
}

