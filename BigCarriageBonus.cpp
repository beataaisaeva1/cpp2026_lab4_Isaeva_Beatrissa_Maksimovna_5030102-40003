#include "BigCarriageBonus.h"

#include "Game.h"

BigCarriageBonus::BigCarriageBonus(float x, float y) : Bonus(x, y) {
}

void BigCarriageBonus::activate(Game& game) {
    game.makeCarriageBig();
}

