#pragma once
#include "Collectible.h"

namespace fp
{
	class FireFruit : public Collectible
	{
	public:
		FireFruit();

		void onCollect(Player& player) override;
	};
}

