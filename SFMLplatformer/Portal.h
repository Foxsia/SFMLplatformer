#pragma once
#include "Collectible.h"
#include "Player.h"

namespace fp
{
	class Portal : public Collectible
	{
	public:
		Portal(int pairId = 0);
		int getPairId() const { return pairId; };

		void setLinkedPortal(Portal* portal);

		void onCollect(Player& player) override;

	private:
		int pairId;
		Portal* linkedPortal;
	};
}

