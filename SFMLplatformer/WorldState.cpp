#include "WorldState.h"
#include "GameContext.h"
#include "Player.h"
#include "TileMap.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include "CameraController.h"

namespace fp
{
	void WorldState::update(float dt, GameContext& context)
	{
		handlePlayerInput(dt, context);

		onWorldUpdate(dt, context);

		updatePlayer(dt, context);

		CollisionSystem::resolvePlayerTileCollision(
			*context.player,
			*context.tileMap
		);

		CollisionSystem::resolveWorldBounds(
			*context.player,
			*context.window
		);

		CameraController::followPlayer(
			*context.camera,
			*context.player,
			*context.tileMap
		);
	}

	void WorldState::onWorldUpdate(float dt, GameContext& context)
	{
	}

	void fp::WorldState::updatePlayer(float dt, GameContext& context)
	{
		context.player->update(dt);
	}
	void WorldState::handlePlayerInput(float dt, GameContext& context)
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
}
