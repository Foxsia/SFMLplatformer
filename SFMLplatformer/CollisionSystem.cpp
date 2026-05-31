#include "CollisionSystem.h"

void fp::CollisionSystem::resolvePlayerTileCollision(Player& player, TileMap& map)
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

void fp::CollisionSystem::resolveEnemyTileCollision(Enemy& enemy, TileMap& map)
{
	const sf::FloatRect bounds = enemy.getShape().getGlobalBounds();

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
				enemy.getShape().setPosition(bounds.left, tileBounds.top - bounds.height);
				enemy.setVelocityY(0.f);
			}
		}
	}
}

void fp::CollisionSystem::resolveWorldBounds(Player& player, sf::RenderWindow& window)
{
	if (player.getPosition().y + player.getGlobalBounds().height > window.getSize().y)
	{
		player.setCanJump(true);
		player.resetVelocityY();
		player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
	}
}
