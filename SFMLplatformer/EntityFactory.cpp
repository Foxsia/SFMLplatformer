#include "EntityFactory.h"

namespace fp
{
	std::unordered_map<int, std::unique_ptr<IEntityCreator>> EntityFactory::creators;

	void EntityFactory::registerCreator(int id, std::unique_ptr<IEntityCreator> creator)
	{
		creators[id] = std::move(creator);
	}
	bool EntityFactory::create(int id, unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
		auto it = creators.find(id);

		if (it == creators.end()) return false;

		it->second->create(x, y, tileMap, enemies, collectibles);

		return true;
	}
}