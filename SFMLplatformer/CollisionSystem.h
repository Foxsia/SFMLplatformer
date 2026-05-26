#pragma once
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
	class CollisionSystem
	{
	public:
		static void resolvePlayerTileCollision(Player& player, TileMap& map);
		static void resolveWorldBounds(Player& player, sf::RenderWindow& window);
	};
}
