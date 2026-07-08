#include "WinState.h"
#include "ScoreManager.h"
#include "Utils.h"

namespace fp
{
	WinState::WinState()
	{
		w_backgroundTexture.loadFromFile("assets/winBackground.png");
		w_backgroundSprite.setTexture(w_backgroundTexture);
	}
	void WinState::update(float dt, GameContext& context)
	{
		if (typingName)
		{
			if (context.input->isActionPressed("SUBMIT_SCORE"))
			{
				ScoreEntry entry;
				entry.name = nameInput;
				entry.score = context.player->getScore();
				entry.time = formatTime(context.game->getLevelRunTime());
				entry.level = formatLevelName(*context.currentLevel);
				ScoreManager::addScore(entry);

				typingName = false;
				savedScore = true;
			}
			return;
		}

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
	void WinState::render(sf::RenderWindow& window, GameContext& context)
	{
		window.setView(window.getDefaultView());

		sf::Vector2u windowSize = window.getSize();
		sf::Vector2u textureSize = w_backgroundTexture.getSize();

		w_backgroundSprite.setScale(
			static_cast<float>(windowSize.x) / textureSize.x,
			static_cast<float>(windowSize.y) / textureSize.y
		);

		window.draw(w_backgroundSprite);


		text.setFont(*context.font);

		if (typingName)
			text.setString("YOU WIN\n\n ENTER NAME + PRESS TAB");
		else
			text.setString("YOU WIN\n\nScore: " + std::to_string(context.player->getScore()) + "\nENTER - RESTART\nM - MENU");

		text.setCharacterSize(40);
		text.setPosition(200.f, 200.f);

		window.draw(text);

		if (typingName)
		{
			nameText.setFont(*context.font);
			nameText.setString("ENTER NAME: " + nameInput);
			nameText.setCharacterSize(30);
			nameText.setPosition(100.f, 100.f);

			window.draw(nameText);
		}
	}
	void WinState::handleEvent(const sf::Event& event)
	{
		if (!typingName) return;

		if (event.type == sf::Event::TextEntered)
		{
			char c = static_cast<char>(event.text.unicode);

			if (std::isprint(c))
				nameInput += c;
		}

		if (event.type == sf::Event::KeyPressed &&
			event.key.code == sf::Keyboard::BackSpace)
		{
			if (!nameInput.empty())
				nameInput.pop_back();
		}
	}
}
