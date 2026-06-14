#pragma once
#include "IState.h"
#include "HUD.h"
#include <memory>

namespace fp
{
	struct GameContext;

	class WorldState : public IState
	{
	protected:
		std::unique_ptr<HUD> hud;

		void updatePlayer(float dt, GameContext& context);

		void handlePlayerInput(float dt, GameContext& context);
	public:
		void update( float dt, GameContext& context) override;
		virtual void handleInput(float dt, GameContext& context);
		void render(sf::RenderWindow& window, GameContext& context) override;
	};
}
