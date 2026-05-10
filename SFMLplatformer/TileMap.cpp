#include "TileMap.h"

TileMap::TileMap()
{
	tileSheet = NULL;
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

void TileMap::addTile(unsigned x, unsigned y)
{
	if (x < tiles.size() && x >= 0)
	{
		if (y < tiles[x].size() && y >= 0)
		{
			if (tiles[x][y] == nullptr)
			{
				tiles[x][y] = new Tile(x, y, tileSize, tileSheet, sf::IntRect(0, 0, tileSize, tileSize), false);
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

void TileMap::update()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int k = 0; k < tiles[i].size(); k++)
		{
			if (tiles[i][k] != nullptr) tiles[i][k]->update();
		}
	}
}

void TileMap::render(sf::RenderTarget& target)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int k = 0; k < tiles[i].size(); k++)
		{
			if(tiles[i][k] != nullptr) tiles[i][k]->render(target);
		}
	}
}
