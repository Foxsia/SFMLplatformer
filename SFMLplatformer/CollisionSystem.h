#pragma once
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"

namespace fp
{
	class CollisionSystem
	{
	public:
		static void resolvePlayerTileCollision(Player& player, TileMap& map);
		static void resolveEnemyTileCollision(Enemy& enemy, TileMap& map);
		static void resolvePlayerEnemyCollision(Player& player, std::vector<Enemy*>& enemies);
		static void resolveWorldBounds(Player& player, sf::RenderWindow& window, const GameContext& context);
	};
}
