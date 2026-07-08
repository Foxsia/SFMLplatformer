#pragma once
#include "WorldState.h"

namespace fp
{
    class PlayState : public WorldState
    {
    public:
        void update(float dt, GameContext& context) override;
        void render(sf::RenderWindow& window, GameContext& context) override;
    };
}

