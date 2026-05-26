#pragma once
#include "IState.h"

namespace fp
{
	class WorldState : public IState
	{
	protected:
		void updatePlayer(float dt, GameContext& context);

		void updateTileCollision(GameContext& context);

		void updateWorldCollision(GameContext& context);

		void updateCamera(GameContext& context);

		void handlePlayerInput(float dt, GameContext& context);
	};
}
