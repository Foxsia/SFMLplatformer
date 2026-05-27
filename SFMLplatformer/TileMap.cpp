#include "TileMap.h"
#include <fstream>

namespace fp
{
	namespace
	{
		const unsigned TILE_TEXTURE_X = 0;
		const unsigned TILE_TEXTURE_Y = 0;
	}

	TileMap::TileMap()
	{
		tileSheet = nullptr;
		tileSize = 0;
	}

	TileMap::TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size)
	{
		tileSheet = tile_sheet;
		tileSize = tile_size;

		tiles.resize(width);
		for (int i = 0; i < tiles.size(); i++)
		{
			tiles[i].resize(height, nullptr);
		}
	}

	TileMap::~TileMap()
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int k = 0; k < tiles[i].size(); k++)
			{
				delete tiles[i][k];
				tiles[i][k] = nullptr;
			}
		}
	}

	Tile* TileMap::getTile(unsigned x, unsigned y)
	{
		if (x < tiles.size())
		{
			if (y < tiles[x].size())
			{
				return tiles[x][y];
			}
		}

		return nullptr;
	}

	void TileMap::addTile(unsigned x, unsigned y)
	{
		if (x < tiles.size() && x >= 0)
		{
			if (y < tiles[x].size() && y >= 0)
			{
				if (tiles[x][y] == nullptr)
				{
					tiles[x][y] = new Tile(x, y, tileSheet, sf::IntRect(TILE_TEXTURE_X, TILE_TEXTURE_Y, tileSize, tileSize));
				}
			}
		}
	}

	void TileMap::removeTile(unsigned x, unsigned y)
	{
		if (x < tiles.size())
		{
			if (y < tiles[x].size())
			{
				if (tiles[x][y] != nullptr)
				{
					delete tiles[x][y];
					tiles[x][y] = nullptr;
				}
			}
		}
	}

	void TileMap::render(sf::RenderTarget& target)
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int k = 0; k < tiles[i].size(); k++)
			{
				if (tiles[i][k] != nullptr) tiles[i][k]->render(target);
			}
		}
	}

	void TileMap::saveToFile(const std::string& filename, const std::vector<Enemy*>& enemies)
	{
		std::ofstream file(filename);
		if (!file.is_open()) return;

		for (size_t y = 0; y < tiles[0].size(); y++)
		{
			for (size_t x = 0; x < tiles.size(); x++)
			{
				int value = 0;

				// tile
				if (tiles[x][y] != nullptr)
				{
					value = 1;
				}

				// enemy
				for (auto enemy : enemies)
				{
					sf::Vector2f pos = enemy->getPosition();

					unsigned enemyX =
						static_cast<unsigned>(pos.x) / tileSize;

					unsigned enemyY =
						static_cast<unsigned>(pos.y) / tileSize;

					if (enemyX == x && enemyY == y)
					{
						value = 2;
					}
				}

				file << value << " ";
			}

			file << "\n";
		}
	}
}
