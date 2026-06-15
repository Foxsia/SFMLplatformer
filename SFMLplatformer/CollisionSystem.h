#pragma once
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Collectible.h"
#include "MovingPlatform.h"

namespace fp
{
	class CollisionSystem
	{
	public:
		static void resolvePlayerTileCollision(Player& player, TileMap& map);
		static void resolvePlayerMovingPlatform(Player& player, std::vector<std::unique_ptr<MovingPlatform>>& platforms);
		static void resolveEnemyTileCollision(Enemy& enemy, TileMap& map);
		static void resolvePlayerEnemyCollision(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
		static void resolvePlayerCollectibleCollision(Player& player, std::vector<std::unique_ptr<Collectible>>& collectibles);
		static void resolveWorldBounds(Player& player, sf::RenderWindow& window, const GameContext& context);
	};
}
