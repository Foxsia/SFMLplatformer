#pragma once
#include "Collectible.h"

namespace fp
{
	class Player;

	class LifeFruit : public Collectible
	{
	public:
		LifeFruit();

		void onCollect(Player& player) override;
	};
}

