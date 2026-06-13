#include "ExtraBallBonus.h"

#include "Game.h"

ExtraBallBonus::ExtraBallBonus(float x, float y) : Bonus(x, y) {
}

void ExtraBallBonus::activate(Game& game) {
    game.addExtraBall();
}

