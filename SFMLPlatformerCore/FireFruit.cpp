#include "FireFruit.h"
#include "Player.h"
#include "ConfigManager.h"

namespace fp
{
	FireFruit::FireFruit()
	{
		texture.loadFromFile("assets/fire_fruit.png");

		sprite.setTexture(texture);
	}
	void FireFruit::onCollect(Player& player)
	{
		const auto& cfg = ConfigManager::get("collectibles");
		player.setFirePower(true);
		player.addScore(cfg.at("fireFruit").get<int>());

		collected = true;
	}
}
