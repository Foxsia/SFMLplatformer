#include "Portal.h"

namespace fp
{
	Portal::Portal(int pairId) : pairId(pairId), linkedPortal(nullptr)
	{
		texture.loadFromFile("assets/world_tileset.png");

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 320, 32, 32));
	}
	void Portal::setLinkedPortal(Portal* portal)
	{
		linkedPortal = portal;
	}
	void Portal::onCollect(Player& player)
	{
		if (player.getPortalCooldown() > 0.f) return;
		if (!linkedPortal) return;
		if (linkedPortal) player.setPosition(linkedPortal->getPosition().x - linkedPortal->getGlobalBounds().width,
			linkedPortal->getPosition().y - player.getGlobalBounds().height / 2.f);

		player.setPortalCooldown(0.5f);
	}
}