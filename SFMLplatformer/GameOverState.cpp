#include "GameOverState.h"
#include "LevelLoader.h"
#include "GameContext.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Game.h"

namespace fp
{
	GameOverState::GameOverState()
	{
		gO_backgroundTexture.loadFromFile("assets/gameOverBackground.png");
		gO_backgroundSprite.setTexture(gO_backgroundTexture);
	}
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

			context.game->restartLevelTimer();

			context.stateManager->changeState(StateType::Play);
		}

		if (context.input->isActionPressed("MENU_BACK"))
		{
			context.stateManager->changeState(StateType::Menu);
		}
	}
	void GameOverState::render(sf::RenderWindow& window, GameContext& context)
	{
		window.setView(window.getDefaultView());

		sf::Vector2u windowSize = window.getSize();
		sf::Vector2u textureSize = gO_backgroundTexture.getSize();

		gO_backgroundSprite.setScale(
			static_cast<float>(windowSize.x) / textureSize.x,
			static_cast<float>(windowSize.y) / textureSize.y
		);

		window.draw(gO_backgroundSprite);

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
