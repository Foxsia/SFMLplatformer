#pragma once
#include "Tile.h"
#include "Enemy.h"
#include "Collectible.h"
#include <vector>

namespace fp
{
	class TileMap
	{
	public:
		TileMap();
		TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size);
		virtual ~TileMap();

		inline const unsigned& getTileSize() const { return tileSize; }

		inline unsigned getWidth() const
		{
			return tiles.size();
		}
		inline unsigned getHeight() const
		{
			return tiles.empty() ? 0 : tiles[0].size();
		}

		Tile* getTile(unsigned x, unsigned y);
		sf::Vector2f getWorldSize() const;

		void addTile(unsigned x, unsigned y);
		void removeTile(unsigned x, unsigned y);

		void clear();

		void render(sf::RenderTarget& target);
		void saveToFile(const std::string& filename, const std::vector<Enemy*>& enemies, const std::vector<Collectible*>& collectibles);

		void loadBackground(const std::string& path);

		void setPlayerSpawn(float x, float y);
		void removePlayerSpawn();
		bool hasSpawn() const;
		const sf::Vector2f& getPlayerSpawn() const;

	private:
		std::vector< std::vector<Tile*> > tiles;
		sf::Texture* tileSheet;
		unsigned tileSize;

		sf::Texture backgroundTexture;
		sf::Sprite backgroundSprite;

		bool hasPlayerSpawn = false;
		sf::Vector2f playerSpawn;
	};
}

