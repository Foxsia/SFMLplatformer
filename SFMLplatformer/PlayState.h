#pragma once
#include "IState.h"

namespace fp
{
    class PlayState : public IState
    {
    public:
        void update(float dt, GameContext& context) override;
        void render(sf::RenderWindow& window, GameContext& context) override;

        void updateInput(float dt, GameContext& context);
        void updateTileCollision(GameContext& context);
        void updateCollision(GameContext& context);
        void updateCamera(GameContext& context);
    };
}

