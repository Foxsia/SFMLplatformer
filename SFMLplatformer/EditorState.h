#pragma once
#include "WorldState.h"

namespace fp
{
	class EditorState : public WorldState
	{
	public:
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		void handleEditorInput(GameContext& context);
	};
}