#pragma once

#include "Bonus.h"

class BottomShieldBonus : public Bonus {
public:
    BottomShieldBonus(float x, float y);
    void activate(Game& game) override;
};

