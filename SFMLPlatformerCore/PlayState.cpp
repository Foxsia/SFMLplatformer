#include "PlayState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "GameOverState.h"
#include "Game.h"

namespace fp
{
    void PlayState::update(float dt, GameContext& context)
    {
        WorldState::update(dt, context);
        
        for (auto& collectible : *context.collectibles)
        {
            if (auto* goal = dynamic_cast<Goal*>(collectible.get()))
            {
                if (goal->isCollected())
                {
                    context.game->setShowDebug(false);
                    context.stateManager->changeState(StateType::Win);
                    return;
                }
            }
        }

        if (context.levelTime <= 0.f)
        {
            context.stateManager->changeState(StateType::GameOver);
            return;
        }

        if (!context.player->isAlive())
        {
            context.game->setShowDebug(false);
            context.stateManager->changeState(StateType::GameOver);
        }
    }
    void PlayState::render(sf::RenderWindow& window, GameContext& context)
    {
        WorldState::render(window, context);
    }
}