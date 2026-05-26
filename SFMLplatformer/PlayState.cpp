#include "PlayState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    void PlayState::update(float dt, GameContext& context)
    {
		handlePlayerInput(dt, context);

		updatePlayer(dt, context);

		updateTileCollision(context);

		updateWorldCollision(context);

		updateCamera(context);
    }

    void PlayState::render(sf::RenderWindow& window, GameContext& context)
    {
        context.tileMap->render(window);
        context.player->render(window);
    }
}