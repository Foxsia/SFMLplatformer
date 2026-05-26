#pragma once
#include "WorldState.h"

namespace fp
{
    class PlayState : public WorldState
    {
    public:
        void render(sf::RenderWindow& window, GameContext& context) override;
    };
}

