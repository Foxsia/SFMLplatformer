#pragma once
#include "IState.h"

namespace fp
{
	class MenuState : public IState
	{
	public:
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
	};
}
