#pragma once
#include "IState.h"

namespace fp
{
	class MenuState : public IState
	{
	public:
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;
	private:
		void handleNavigation(GameContext& context);
		void selectCurrentOption(GameContext& context);
		void loadLevel(GameContext& context);
		void openEditor(GameContext& context);
	};
}
