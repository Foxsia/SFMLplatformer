#pragma once
#include "Collectible.h"

namespace fp
{
	class Player;

	class Goal : public Collectible
	{
	public:
		Goal();

		void onCollect(Player& player) override;

		bool isReached() const { return reached; }

		bool isGoal() const override { return true; }
	private:
		bool reached = false;
	};
}

