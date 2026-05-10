#pragma once
#include "Tile.h"
#include <vector>

class TileMap
{
private:
	std::vector< std::vector<Tile*> > tiles;
public:
	TileMap();
	virtual ~TileMap();

	//func
	void addTile(unsigned x, unsigned y);
	void removeTile(unsigned x, unsigned y);

	void update();
	void render();
};

