#pragma once

#include "Bonus.h"

class SlowBallBonus : public Bonus {
public:
    SlowBallBonus(float x, float y);

    void activate(Game& game) override;
};

