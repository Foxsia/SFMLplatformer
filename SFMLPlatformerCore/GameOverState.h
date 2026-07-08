#pragma once
#include "IState.h"

namespace fp
{
	class GameOverState : public IState
	{
	public:
		GameOverState();
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		sf::Texture gO_backgroundTexture;
		sf::Sprite gO_backgroundSprite;
	};
}

