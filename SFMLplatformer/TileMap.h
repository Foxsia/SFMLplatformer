#pragma once
#include "Tile.h"
#include <vector>

class TileMap
{
private:
	std::vector< std::vector<Tile*> > tiles;
	sf::Texture* tileSheet;
	unsigned tileSize;

public:
	TileMap();
	TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size);
	virtual ~TileMap();

	//inline func
	inline const unsigned& getTileSize() const { return tileSize; }

	inline unsigned getWidth() const
	{
		return tiles.size();
	}
	inline unsigned getHeight() const
	{
		return tiles.empty() ? 0 : tiles[0].size();
	}

	//getters
	Tile* getTile(unsigned x, unsigned y);
	
	//func
	void addTile(unsigned x, unsigned y);
	void removeTile(unsigned x, unsigned y);

	void render(sf::RenderTarget& target);
};

