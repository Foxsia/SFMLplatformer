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
			Enemy,
			Player
		};
		BrushType brush = BrushType::Tile;
		void updateBrush(GameContext& context);
		void addElement(GameContext& context, int mouseX, int mouseY);
		void removeElement(GameContext& context, int mouseX, int mouseY);
		void removeEnemyAtPosition(GameContext& context, const sf::Vector2f& pos);
		void handleInput(float dt, GameContext& context) override;
	};
}