#pragma once
#include "WorldState.h"

namespace fp
{
	class EditorState : public WorldState
	{
	protected:
		void onWorldUpdate(float dt, GameContext& context) override;
	public:
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		void handleEditorInput(GameContext& context);
	};
}