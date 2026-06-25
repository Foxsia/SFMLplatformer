#include "CollisionSystem.h"
#include "EditorState.h"
#include <algorithm>

namespace fp
{
	namespace
	{
		const int SCORE_ENEMY = 100;
		const int SCORE_COLLECTIBLE = 50;
	}
	namespace
	{
		const float COLLISION_TOLERANCE = 10.f;
	}
	void CollisionSystem::resolvePlayerTileCollision(Player& player, TileMap& map)
	{
		bool grounded = false;

		sf::FloatRect playerBounds = player.getHitbox();

		const int TILE_SIZE = map.getTileSize();

		int leftTile = static_cast<int>(playerBounds.left / TILE_SIZE);
		int rightTile = static_cast<int>((playerBounds.left + playerBounds.width) / TILE_SIZE);
		int topTile = static_cast<int>(playerBounds.top / TILE_SIZE);
		int bottomTile = static_cast<int>((playerBounds.top + playerBounds.height) / TILE_SIZE);

		leftTile--;
		rightTile++;
		topTile--;
		bottomTile++;

		leftTile = std::max(0, leftTile);
		rightTile = std::min((int)map.getWidth() - 1, rightTile);
		topTile = std::max(0, topTile);
		bottomTile = std::min((int)map.getHeight() - 1, bottomTile);

		for (int x = leftTile; x <= rightTile; x++)
		{
			for (int y = topTile; y <= bottomTile; y++)
			{
				Tile* tile = map.getTile(x, y);

				if (!tile) continue;

				const sf::FloatRect tileBounds = tile->getGlobalBounds();

				if (!playerBounds.intersects(tileBounds)) continue;

				// collision only from top
				float playerBottom = playerBounds.top + playerBounds.height;
				float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

				bool insideTileX =
					playerCenterX > tileBounds.left &&
					playerCenterX < tileBounds.left + tileBounds.width;

				if (insideTileX && player.getVelocity().y >= 0.f && playerBottom <= tileBounds.top + 20.f)
				{
					playerBounds = player.getGlobalBounds();
					player.setPosition(
						playerBounds.left,
						tileBounds.top - playerBounds.height
					);

					player.resetVelocityY();

					grounded = true;
				}
			}
		}

		player.setCanJump(player.getCanJump() || grounded);
	}

	void CollisionSystem::resolvePlayerMovingPlatform(
		Player& player,
		std::vector<std::unique_ptr<MovingPlatform>>& platforms)
	{
		sf::FloatRect playerBounds = player.getHitbox();

		const float playerBottom = playerBounds.top + playerBounds.height;

		bool onPlatform = false;

		for (auto& platform : platforms)
		{
			for (auto tile : platform->getTiles())
			{
				sf::FloatRect tileBounds = tile->getGlobalBounds();

				if (!playerBounds.intersects(tileBounds)) continue;

				float tileTop = tileBounds.top;

				bool standing = player.getVelocity().y >= 0.f && playerBottom <= tileTop + COLLISION_TOLERANCE &&
					playerBottom >= tileTop - COLLISION_TOLERANCE / 2.f;

				if (standing)
				{
					playerBounds = player.getGlobalBounds();
					player.setPosition(
						player.getPosition().x + platform->getDelta().x,
						tileTop - playerBounds.height
					);

					player.resetVelocityY();
					onPlatform = true;
					break;
				}
			}
		}
		player.setCanJump(player.getCanJump() || onPlatform);
	}

	void CollisionSystem::resolveEnemyTileCollision(Enemy& enemy, TileMap& map)
	{
		const sf::FloatRect bounds = enemy.getGlobalBounds();

		const int TILE_SIZE = map.getTileSize();

		int leftTile = static_cast<int>(bounds.left / TILE_SIZE);
		int rightTile = static_cast<int>((bounds.left + bounds.width) / TILE_SIZE);
		int topTile = static_cast<int>(bounds.top / TILE_SIZE);
		int bottomTile = static_cast<int>((bounds.top + bounds.height) / TILE_SIZE);

		leftTile--;
		rightTile++;
		topTile--;
		bottomTile++;

		leftTile = std::max(0, leftTile);
		rightTile = std::min((int)map.getWidth() - 1, rightTile);
		topTile = std::max(0, topTile);
		bottomTile = std::min((int)map.getHeight() - 1, bottomTile);

		for (int x = leftTile; x <= rightTile; x++)
		{
			for (int y = topTile; y <= bottomTile; y++)
			{
				Tile* tile = map.getTile(x, y);
				if (!tile) continue;

				const sf::FloatRect tileBounds = tile->getGlobalBounds();

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

	void CollisionSystem::resolveFireballTileCollision(std::vector<std::unique_ptr<FireBall>>& fireballs, TileMap& map)
	{
		const int TILE_SIZE = map.getTileSize();

		for (auto& fireball : fireballs)
		{
			sf::FloatRect bounds = fireball->getBounds();

			int leftTile = static_cast<int>(bounds.left / TILE_SIZE);
			int rightTile = static_cast<int>((bounds.left + bounds.width) / TILE_SIZE);
			int topTile = static_cast<int>(bounds.top / TILE_SIZE);
			int bottomTile = static_cast<int>((bounds.top + bounds.height) / TILE_SIZE);

			bool handled = false;

			for (int x = leftTile; x <= rightTile && !handled; x++)
			{
				for (int y = topTile; y <= bottomTile && !handled; y++)
				{
					Tile* tile = map.getTile(x, y);
					if (!tile) continue;

					const sf::FloatRect tileBounds = tile->getGlobalBounds();

					if (!bounds.intersects(tileBounds)) continue;

					float fireballBottom = bounds.top + bounds.height;

					if (fireball->getVelocity().y > 0.f && fireballBottom <= tileBounds.top + COLLISION_TOLERANCE)
					{
						fireball->setPosition(
							bounds.left,
							tileBounds.top - bounds.height
						);

						fireball->bounce();
					}
					else
					{
						fireball->destroy();
					}

					handled = true;
					break;
				}
			}
		}
	}

	void CollisionSystem::resolvePlayerEnemyCollision(Player& player, std::vector < std::unique_ptr<Enemy>>& enemies)
	{
		sf::FloatRect playerBounds = player.getHitbox();

		for (const auto& enemy : enemies)
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

				if (!enemy->isAlive()) player.addScore(SCORE_ENEMY);

				player.resetVelocityY();
				continue;
			}

			player.takeDamage(1);
			if (player.hasFirePower()) player.setFirePower(false);

			if (player.getHealth() <= 0)
			{
				player.loseLife(1);
			}
		}
	}

	void CollisionSystem::resolveEnemyFireballCollision(std::vector<std::unique_ptr<FireBall>>& fireballs, std::vector<std::unique_ptr<Enemy>>& enemies)
	{
		for (auto& fireball : fireballs)
		{
			if (!fireball->isAlive()) continue;

			for (auto& enemy : enemies)
			{
				if (!enemy->isAlive()) continue;

				if (fireball->getBounds().intersects(enemy->getHitbox()))
				{
					enemy->takeDamage(1);

					fireball->destroy();

					break;
				}
			}
		}
	}

	void CollisionSystem::resolvePlayerCollectibleCollision(Player& player, std::vector < std::unique_ptr<Collectible>>& collectibles)
	{
		for (auto& collectible : collectibles)
		{
			if (collectible->isCollected()) continue;

			if (player.getHitbox().intersects(collectible->getGlobalBounds()))
			{
				collectible->onCollect(player);
			}
		}
	}

	void CollisionSystem::resolveWorldBounds(Player& player, sf::RenderWindow& window, const GameContext& context)
	{
		float playerBottom = player.getPosition().y + player.getHitbox().height;

		float windowBottom = static_cast<float>(window.getSize().y);

		if (playerBottom > windowBottom)
		{
			player.loseLife(player.getLives());
			return;
		}
	}
	void CollisionSystem::resolveFireballBounds(std::vector<std::unique_ptr<FireBall>>& fireballs, sf::RenderWindow& window)
	{
		float screenBottom = static_cast<float>(window.getSize().y);

		for (auto& fireball : fireballs)
		{
			if (!fireball->isAlive()) continue;

			float fireballBottom =
				fireball->getBounds().top +
				fireball->getBounds().height;

			if (fireballBottom > screenBottom)
			{
				fireball->destroy();
			}
		}
	}
}
