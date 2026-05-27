#include "PlayState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    void PlayState::render(sf::RenderWindow& window, GameContext& context)
    {
        WorldState::render(window, context);
    }
}