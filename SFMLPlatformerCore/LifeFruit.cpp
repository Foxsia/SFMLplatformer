#include "LifeFruit.h"
#include "Player.h"
#include "ConfigManager.h"

namespace fp
{
	LifeFruit::LifeFruit()
	{
		texture.loadFromFile("assets/fruit.png");

		sprite.setTexture(texture);
	}
	void LifeFruit::onCollect(Player& player)
	{
		const auto& cfg = ConfigManager::get("collectibles");
		player.addLife(1);
		player.addScore(cfg.at("lifeFruit").get<int>());

		collected = true;
	}
}
