#pragma once

#include "Bonus.h"

class ExtraBallBonus : public Bonus {
public:
    ExtraBallBonus(float x, float y);

    void activate(Game& game) override;
};

