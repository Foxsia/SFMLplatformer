#include "PlayState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    void PlayState::update(float dt, GameContext& context)
    {
        updateInput(dt, context);
        context.player->update(dt);
        updateTileCollision(context);
        updateCollision(context);
        updateCamera(context);
    }

    void PlayState::render(sf::RenderWindow& window, GameContext& context)
    {
        context.tileMap->render(window);
        context.player->render(window);
    }
	void PlayState::updateInput(float dt, GameContext& context)
	{
		sf::RenderWindow& window = *context.window;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

		Player* player = context.player;
		TileMap* tileMap = context.tileMap;

		//player movement
		if (context.input->isKeyDown("MOVE_LEFT"))
		{
			player->move(-1.f, 0.f, dt);
		}
		else if (context.input->isKeyDown("MOVE_RIGHT"))
		{
			player->move(1.f, 0.f, dt);
		}

		if (context.input->isKeyDown("JUMP") && player->getCanJump())
		{
			player->jump();
		}
	}
	void PlayState::updateTileCollision(GameContext& context)
	{
		Player* player = context.player;
		TileMap* tileMap = context.tileMap;

		bool grounded = false;

		sf::FloatRect playerBounds = player->getGlobalBounds();

		for (int x = 0; x < tileMap->getWidth(); x++)
		{
			for (int y = 0; y < tileMap->getHeight(); y++)
			{
				Tile* tile = tileMap->getTile(x, y);

				if (!tile) continue;

				sf::FloatRect tileBounds = tile->getHitbox();

				if (!playerBounds.intersects(tileBounds)) continue;

				// collision only from top
				float playerBottom = playerBounds.top + playerBounds.height;
				float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

				bool insideTileX =
					playerCenterX > tileBounds.left &&
					playerCenterX < tileBounds.left + tileBounds.width;

				if (insideTileX && player->getVelocity().y >= 0.f && playerBottom <= tileBounds.top + 20.f)
				{
					player->setPosition(
						playerBounds.left,
						tileBounds.top - playerBounds.height
					);

					player->resetVelocityY();

					grounded = true;
				}
			}
		}

		player->setCanJump(grounded);
	}
	void PlayState::updateCollision(GameContext& context)
	{
		if (context.player->getPosition().y + context.player->getGlobalBounds().height > context.window->getSize().y)
		{
			context.player->setCanJump(true);
			context.player->resetVelocityY();
			context.player->setPosition(context.player->getPosition().x, context.window->getSize().y - context.player->getGlobalBounds().height);
		}
	}
	void PlayState::updateCamera(GameContext& context)
	{
		sf::FloatRect playerBounds = context.player->getGlobalBounds();

		float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

		float cameraCenterX = context.camera->getCenter().x;

		float halfWidth = context.camera->getSize().x / 2.f;

		// deadzone
		float rightBorder = cameraCenterX + (context.camera->getSize().x / 2.f);

		float leftBorder = cameraCenterX - (context.camera->getSize().x / 2.f);

		// move camera right
		if (playerCenterX > rightBorder)
		{
			cameraCenterX = playerCenterX - (context.camera->getSize().x / 2.f);
		}

		// move camera left
		if (playerCenterX < leftBorder)
		{
			cameraCenterX = playerCenterX + (context.camera->getSize().x / 2.f);
		}

		// map bounds
		float mapWidth = context.tileMap->getWidth() * context.tileMap->getTileSize();

		if (cameraCenterX < halfWidth) cameraCenterX = halfWidth;

		if (cameraCenterX > mapWidth - halfWidth) cameraCenterX = mapWidth - halfWidth;

		context.camera->setCenter(
			cameraCenterX,
			context.camera->getSize().y / 2.f
		);
	}
}