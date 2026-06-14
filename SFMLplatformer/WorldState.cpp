#include "WorldState.h"
#include "GameContext.h"
#include "Player.h"
#include "TileMap.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include "CameraController.h"
#include "Enemy.h"

namespace fp
{
	void WorldState::update(float dt, GameContext& context)
	{
		handleInput(dt, context);

		if (!context.isEditor) updatePlayer(dt, context);

		CollisionSystem::resolvePlayerTileCollision(
			*context.player,
			*context.tileMap
		);

		CollisionSystem::resolveWorldBounds(
			*context.player,
			*context.window,
			context
		);

		CameraController::followPlayer(
			*context.camera,
			*context.player,
			*context.tileMap
		);

		if (!context.isEditor)
		{
			for (auto& enemy : *context.enemies)
			{
				enemy->update(dt, *context.tileMap);
			}
		}

		CollisionSystem::resolvePlayerEnemyCollision(*context.player, *context.enemies);
		CollisionSystem::resolvePlayerCollectibleCollision(*context.player, *context.collectibles);
	}

	void WorldState::handleInput(float dt, GameContext& context)
	{
		handlePlayerInput(dt, context);
	}

	void WorldState::render(sf::RenderWindow& window, GameContext& context)
	{
		if (!context.isEditor)
		{
			if (!hud)
			{
				hud = std::make_unique<HUD>(*context.font);
			}
		}

		context.tileMap->render(window);

		for (auto& collectible : *context.collectibles)
		{
			collectible->render(window);
		}

		if (!context.isEditor)context.player->render(window);

		for (auto& enemy : *context.enemies)
		{
			enemy->render(window);
		}

		if (!context.isEditor)
		{
			hud->update(*context.player);
			hud->render(window);
		}
	}

	void fp::WorldState::updatePlayer(float dt, GameContext& context)
	{
		context.player->update(dt);
	}
	void WorldState::handlePlayerInput(float dt, GameContext& context)
	{
		sf::RenderWindow& window = *context.window;

		const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

		Player* player = context.player;
		TileMap* tileMap = context.tileMap;

		if (context.input->isActionPressed("MOVE_LEFT"))
		{
			player->move(-1.f, 0.f, dt);
		}
		else if (context.input->isActionPressed("MOVE_RIGHT"))
		{
			player->move(1.f, 0.f, dt);
		}

		if (context.input->isActionPressed("JUMP") && player->getCanJump())
		{
			player->jump();
		}
	}
}
