#include "GameOverState.h"
#include "LevelLoader.h"
#include "GameContext.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Game.h"

namespace fp
{
	void GameOverState::update(float dt, GameContext& context)
	{
		if (context.input->isActionPressed("MENU_SELECT"))
		{
			context.player->hardReset();
			LevelLoader::load(
				*context.currentLevel,
				*context.tileMap,
				*context.player,
				*context.enemies,
				*context.collectibles
			);

			context.game->buildMovingPlatforms();

			delete* context.state;
			*context.state = new PlayState();
		}

		if (context.input->isActionPressed("MENU_BACK"))
		{
			delete* context.state;
			*context.state = new MenuState();
		}
	}
	void GameOverState::render(sf::RenderWindow& window, GameContext& context)
	{
		window.setView(window.getDefaultView());

		sf::Text text;
		text.setFont(*context.font);
		text.setString(
			"GAME OVER\n\n"
			"Score: " + std::to_string(context.player->getScore()) +
			"\nENTER - Restart\n"
			"M - Menu"
		);

		text.setCharacterSize(40);
		text.setPosition(200.f, 200.f);

		window.draw(text);
	}
}
