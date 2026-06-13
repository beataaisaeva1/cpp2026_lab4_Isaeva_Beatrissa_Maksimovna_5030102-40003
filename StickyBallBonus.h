#pragma once

#include "Bonus.h"

class StickyBallBonus : public Bonus {
public:
    StickyBallBonus(float x, float y);

    void activate(Game& game) override;
};

