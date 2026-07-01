#include "CollisionSystem.h"
#include "EditorState.h"
#include <algorithm>
#include "ConfigManager.h"

namespace fp
{
	void CollisionSystem::resolvePlayerTileCollision(Player& player, TileMap& map)
	{
		const auto& cfg = ConfigManager::get("collision");
		bool grounded = false;

		sf::FloatRect playerBounds = player.getHitbox();

		TileRange range = CalculateTileRange(playerBounds, map);

		for (int x = range.left; x <= range.right; x++)
		{
			for (int y = range.top; y <= range.bottom; y++)
			{
				Tile* tile = map.getTile(x, y);

				if (!tile) continue;

				if (tile->getType() == TileType::Spike)
				{
					continue;
				}

				const sf::FloatRect tileBounds = tile->getGlobalBounds();

				if (!playerBounds.intersects(tileBounds)) continue;

				// collision only from top
				float playerBottom = playerBounds.top + playerBounds.height;
				float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

				bool insideTileX =
					playerCenterX > tileBounds.left &&
					playerCenterX < tileBounds.left + tileBounds.width;

				if (insideTileX && player.getVelocity().y >= 0.f && playerBottom <= tileBounds.top + cfg["groundSnapTolerance"])
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
		const auto& cfg = ConfigManager::get("collision");
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

				bool standing = player.getVelocity().y >= 0.f && playerBottom <= tileTop + cfg["collisionTolerance"] &&
					playerBottom >= tileTop - cfg["collisionTolerance"] / 2.f;

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
		const auto& cfg = ConfigManager::get("collision");
		const sf::FloatRect bounds = enemy.getGlobalBounds();

		TileRange range = CalculateTileRange(bounds, map);

		for (int x = range.left; x <= range.right; x++)
		{
			for (int y = range.top; y <= range.bottom; y++)
			{
				Tile* tile = map.getTile(x, y);
				if (!tile) continue;

				const sf::FloatRect tileBounds = tile->getGlobalBounds();

				if (!bounds.intersects(tileBounds)) continue;

				float enemyBottom = bounds.top + bounds.height;

				// simple ground collision
				if (enemy.getVelocity().y > 0.f &&
					enemyBottom <= tileBounds.top + cfg["collisionTolerance"])
				{
					enemy.setPosition(bounds.left, tileBounds.top - bounds.height);
					enemy.setVelocityY(0.f);
					return;
				}
			}
		}
	}

	void CollisionSystem::resolveFireballTileCollision(std::vector<std::unique_ptr<FireBall>>& fireballs, TileMap& map)
	{
		const auto& cfg = ConfigManager::get("collision");
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

					if (fireball->getVelocity().y > 0.f && fireballBottom <= tileBounds.top + cfg["collisionTolerance"])
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
		const auto& cfg = ConfigManager::get("collision");
		sf::FloatRect playerBounds = player.getHitbox();

		for (const auto& enemy : enemies)
		{
			if (!enemy->isAlive()) continue;

			sf::FloatRect enemyBounds = enemy->getHitbox();

			if (!playerBounds.intersects(enemyBounds)) continue;

			if(player.isInvulnerable())
			{
				enemy->takeDamage(enemy->getMaxHealth());
				enemy->loseLife(enemy->getStartLives());

				if (!enemy->isAlive()) player.addScore(cfg["enemyScore"]);

				continue;
			}

			float playerBottom = playerBounds.top + playerBounds.height;
			float enemyTop = enemyBounds.top;

			bool stomp = player.getVelocity().y > 0.f && playerBottom <= enemyTop + cfg["collisionTolerance"];

			if (stomp)
			{
				enemy->takeDamage(1);

				if (!enemy->isAlive()) player.addScore(cfg["enemyScore"]);

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

	void CollisionSystem::resolveFireballEnemyPlayerCollision(std::vector<std::unique_ptr<FireBall>>& fireballs, Player& player, std::vector<std::unique_ptr<Enemy>>& enemies)
	{
		for (auto& fireball : fireballs)
		{
			if (!fireball->isAlive()) continue;

			if (fireball->getTeam() == Team::Enemy)
			{
				if (fireball->getBounds().intersects(player.getHitbox()))
				{
					player.takeDamage(1);
					fireball->destroy();
					continue;
				}
			}

			if (fireball->getTeam() == Team::Player)
			{
				for (auto& enemy : enemies)
				{
					if (!enemy->isAlive()) continue;

					if (fireball->getTeam() == Team::Player && fireball->getBounds().intersects(enemy->getHitbox()))
					{
						enemy->takeDamage(1);

						fireball->destroy();

						break;
					}
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
	void CollisionSystem::resolvePlayerSpikeCollision(Player& player, TileMap& map)
	{
		sf::FloatRect playerBounds = player.getHitbox();

		TileRange range = CalculateTileRange(playerBounds, map);

		for (int x = range.left; x <= range.right; x++)
		{
			for (int y = range.top; y <= range.bottom; y++)
			{
				Tile* tile = map.getTile(x, y);

				if (!tile) continue;

				if (tile->getType() != TileType::Spike) continue;

				if (!tile->isSpikeActive()) continue;

				if (playerBounds.intersects(tile->getGlobalBounds()))
				{
					player.takeDamage(1);
				}
			}
		}
	}
	bool CollisionSystem::canTeleportTo(const sf::FloatRect& bounds, TileMap& map, std::vector<std::unique_ptr<Enemy>>& enemies)
	{
		for (const auto& enemy : enemies)
		{
			if (!enemy->isAlive()) continue;

			if (bounds.intersects(enemy->getHitbox()))
			{
				return false;
			}
		}

		TileRange range = CalculateTileRange(bounds, map);

		for (int x = range.left; x <= range.right; x++)
		{
			for (int y = range.top; y <= range.bottom; y++)
			{
				Tile* tile = map.getTile(x, y);

				if (!tile) continue;

				if (tile->getType() != TileType::Spike) continue;

				if (bounds.intersects(tile->getGlobalBounds()))
				{
					return false;
				}
			}
		}

		return true;
	}
	CollisionSystem::TileRange CollisionSystem::CalculateTileRange(const sf::FloatRect& bounds, const TileMap& map, int padding)
	{
		const int TILE_SIZE = map.getTileSize();

		CollisionSystem::TileRange range;

		range.left = std::max(0, static_cast<int>(bounds.left / TILE_SIZE) - padding);
		range.right = std::min(static_cast<int>(map.getWidth()) - 1, static_cast<int>((bounds.left + bounds.width) / TILE_SIZE) + padding);
		range.top = std::max(0, static_cast<int>(bounds.top / TILE_SIZE) - padding);
		range.bottom = std::min(static_cast<int>(map.getHeight()) - 1, static_cast<int>((bounds.top + bounds.height) / TILE_SIZE) + padding);

		return range;
	}
}
