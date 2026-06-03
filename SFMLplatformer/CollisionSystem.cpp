#include "CollisionSystem.h"
#include "EditorState.h"

namespace fp
{
	namespace
	{
		const float COLLISION_TOLERANCE = 10.f;
	}
	void CollisionSystem::resolvePlayerTileCollision(Player& player, TileMap& map)
	{
		bool grounded = false;

		sf::FloatRect playerBounds = player.getGlobalBounds();

		for (int x = 0; x < map.getWidth(); x++)
		{
			for (int y = 0; y < map.getHeight(); y++)
			{
				Tile* tile = map.getTile(x, y);

				if (!tile) continue;

				const sf::FloatRect tileBounds = tile->getHitbox();

				if (!playerBounds.intersects(tileBounds)) continue;

				// collision only from top
				float playerBottom = playerBounds.top + playerBounds.height;
				float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

				bool insideTileX =
					playerCenterX > tileBounds.left &&
					playerCenterX < tileBounds.left + tileBounds.width;

				if (insideTileX && player.getVelocity().y >= 0.f && playerBottom <= tileBounds.top + 20.f)
				{
					player.setPosition(
						playerBounds.left,
						tileBounds.top - playerBounds.height
					);

					player.resetVelocityY();

					grounded = true;
				}
			}
		}

		player.setCanJump(grounded);
	}

	void CollisionSystem::resolveEnemyTileCollision(Enemy& enemy, TileMap& map)
	{
		const sf::FloatRect bounds = enemy.getGlobalBounds();

		for (int x = 0; x < map.getWidth(); x++)
		{
			for (int y = 0; y < map.getHeight(); y++)
			{
				Tile* tile = map.getTile(x, y);
				if (!tile) continue;

				const sf::FloatRect tileBounds = tile->getHitbox();

				if (!bounds.intersects(tileBounds)) continue;

				// simple ground collision
				if (enemy.getVelocity().y > 0.f)
				{
					enemy.setPosition(bounds.left, tileBounds.top - bounds.height);
					enemy.setVelocityY(0.f);
				}
			}
		}
	}

	void CollisionSystem::resolvePlayerEnemyCollision(Player& player, std::vector<Enemy*>& enemies)
	{
		sf::FloatRect playerBounds = player.getHitbox();

		for (auto* enemy : enemies)
		{
			if (!enemy->isAlive()) continue;

			sf::FloatRect enemyBounds = enemy->getHitbox();

			if (!playerBounds.intersects(enemyBounds)) continue;

			float playerBottom = playerBounds.top + playerBounds.height;
			float enemyTop = enemyBounds.top;

			bool stomp = player.getVelocity().y > 0.f && playerBottom <= enemyTop + COLLISION_TOLERANCE;

			if (stomp)
			{
				enemy->takeDamage(1);
				player.resetVelocityY();
				continue;
			}

			float playerCenterY = playerBounds.top + playerBounds.height * 0.5f;

			float enemyCenterY = enemyBounds.top + enemyBounds.height * 0.5f;

			bool fromBelow = playerCenterY > enemyCenterY;

			if (fromBelow)
			{
				continue;
			}

			player.takeDamage(1);
		}
	}

	void CollisionSystem::resolveWorldBounds(Player& player, sf::RenderWindow& window, const GameContext& context)
	{
		if (dynamic_cast<EditorState*>(*context.state))
		{
			if (player.getPosition().y + player.getGlobalBounds().height > window.getSize().y)
			{
				player.setCanJump(true);
				player.resetVelocityY();
				player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
			}
			return;
		}

		float playerBottom = player.getPosition().y + player.getGlobalBounds().height;

		float windowBottom = static_cast<float>(window.getSize().y);

		if (playerBottom > windowBottom)
		{
			player.takeDamage(player.getHealth());
			return;
		}
	}
}
