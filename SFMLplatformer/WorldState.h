#pragma once
#include "IState.h"

namespace fp
{
	struct GameContext;

	class WorldState : public IState
	{
	protected:
		virtual void onWorldUpdate( float dt, GameContext& context);

		void updatePlayer(float dt, GameContext& context);

		void handlePlayerInput(float dt, GameContext& context);
	public:
		virtual ~WorldState() = default;

		void update( float dt, GameContext& context) override;
	};
}
