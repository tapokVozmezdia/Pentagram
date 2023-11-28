#pragma once

#include "menu.hpp"

namespace Controls
{
    void moveWASD(const llint t_id, GraphicsHandler* graphics);

    void attackLMC(const llint t_id, GraphicsHandler* graphics);

    void attackRMC(const llint t_id, GraphicsHandler* graphics);

    void vibrate(const llint t_id, GraphicsHandler* graphics, int intencity);
};