#pragma once
#include "IState.h"

namespace fp
{
	struct GameContext;

	class WorldState : public IState
	{
	protected:

		void updatePlayer(float dt, GameContext& context);

		void handlePlayerInput(float dt, GameContext& context);
	public:
		virtual ~WorldState() = default;

		void update( float dt, GameContext& context) override;
		virtual void handleInput(float dt, GameContext& context);
		void render(sf::RenderWindow& window, GameContext& context) override;
	};
}
