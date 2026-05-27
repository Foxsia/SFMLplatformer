#pragma once
#include "WorldState.h"

namespace fp
{
	class EditorState : public WorldState
	{
	public:
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		enum class BrushType
		{
			Tile,
			Enemy
		};
		BrushType brush = BrushType::Tile;
		void handleInput(float dt, GameContext& context) override;
	};
}