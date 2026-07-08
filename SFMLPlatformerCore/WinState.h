#pragma once
#include "IState.h"
#include "LevelLoader.h"
#include "GameContext.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Game.h"

namespace fp
{
	class WinState : public IState
	{
	public:
		WinState();
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
		void handleEvent(const sf::Event& event) override;
		bool blocksGameInput() const { return typingName; }
	private:
		sf::Texture w_backgroundTexture;
		sf::Sprite w_backgroundSprite;

		bool typingName = true;
		std::string nameInput;

		sf::Text text;
		sf::Text nameText;
		bool savedScore = false;
	};
}
