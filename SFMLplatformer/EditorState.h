#pragma once
#include "WorldState.h"
#include "BrushType.h"
#include "EditorHUD.h"

namespace fp
{
	class EditorState : public WorldState
	{
	public:
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		EditorHUD hud;
		BrushType brush = BrushType::Tile;

		int cursorX = 0;
		int cursorY = 0;

		float moveCooldown = 0.f;

		void updateGamepadCursor(float dt, GameContext& context);
		void updateBrush(float dt, GameContext& context);
		void addElement(GameContext& context, int mouseX, int mouseY);
		void removeElement(GameContext& context, int mouseX, int mouseY);
		void removeEnemyAtPosition(GameContext& context, const sf::Vector2f& pos);
		void removeCollectibleAtPosition(GameContext& context, const sf::Vector2f& pos);
		void handleInput(float dt, GameContext& context) override;
	};
}