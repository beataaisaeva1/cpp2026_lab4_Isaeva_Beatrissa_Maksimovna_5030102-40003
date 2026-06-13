#pragma once

#include "Bonus.h"

class BigCarriageBonus : public Bonus {
public:
    BigCarriageBonus(float x, float y);

    void activate(Game& game) override;
};



