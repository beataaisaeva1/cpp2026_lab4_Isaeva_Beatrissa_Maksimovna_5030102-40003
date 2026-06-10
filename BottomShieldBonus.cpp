#include "BottomShieldBonus.h"

#include "Game.h"

BottomShieldBonus::BottomShieldBonus(float x, float y) : Bonus(x, y) {
}

void BottomShieldBonus::activate(Game& game) {
    game.activateShield();
}

