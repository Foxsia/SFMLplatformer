#pragma once
#include "IState.h"
#include "ScoreManager.h"

namespace fp
{
	class MenuState : public IState
	{
	public:
		MenuState();
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		void handleNavigation(GameContext& context);
		void selectCurrentOption(GameContext& context);
		void loadLevel(GameContext& context);
		void openEditor(GameContext& context);

		sf::Texture m_backgroundTexture;
		sf::Sprite m_backgroundSprite;

		bool showScores = false;
		std::vector<ScoreEntry> scores;
	};
}
