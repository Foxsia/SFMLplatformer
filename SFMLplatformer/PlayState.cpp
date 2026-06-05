#include "PlayState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "GameOverState.h"

namespace fp
{
    void PlayState::update(float dt, GameContext& context)
    {
        WorldState::update(dt, context);

        if (!context.player->isAlive())
        {
            delete *context.state;
            *context.state = new GameOverState();
        }
    }
    void PlayState::render(sf::RenderWindow& window, GameContext& context)
    {
        WorldState::render(window, context);
    }
}