#pragma once
#include "IState.h"
#include "HUD.h"

namespace fp
{
	struct GameContext;

	class WorldState : public IState
	{
	protected:
		HUD* hud = nullptr;

		void updatePlayer(float dt, GameContext& context);

		void handlePlayerInput(float dt, GameContext& context);
	public:
		~WorldState();

		void update( float dt, GameContext& context) override;
		virtual void handleInput(float dt, GameContext& context);
		void render(sf::RenderWindow& window, GameContext& context) override;
	};
}
