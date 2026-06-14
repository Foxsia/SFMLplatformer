#include "MenuState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "GameContext.h"
#include "LevelLoader.h"
#include "TileMap.h"
#include "Game.h"
#include <filesystem>

namespace fp
{
	namespace
	{
		const int MENU_INPUT_DELAY_MS = 150;

		const float MENU_X = 250.f;
		const float TITLE_Y = 80.f;
		const float MENU_START_Y = 180.f;
		const float MENU_SPACING = 50.f;
	}

	void MenuState::update(float dt, GameContext& context)
	{
		handleNavigation(context);

		if (context.input->isActionPressed("MENU_SELECT"))
		{
			selectCurrentOption(context);
		}
	}

	void MenuState::render(sf::RenderWindow& window, GameContext& context)
	{
		window.setView(window.getDefaultView());
		window.clear(sf::Color::Black);

		sf::Text title;
		title.setFont(*context.font);
		title.setString("LEVEL SELECT");
		title.setCharacterSize(40);
		title.setFillColor(sf::Color::White);
		title.setPosition(MENU_X, TITLE_Y);
		window.draw(title);

		for (int i = 0; i < context.levelFiles->size(); i++)
		{
			std::filesystem::path path((*context.levelFiles)[i]);

			sf::Text text;
			text.setFont(*context.font);
			text.setString(path.stem().string());
			text.setCharacterSize(28);
			text.setPosition(MENU_X, MENU_START_Y + i * MENU_SPACING);

			i == *context.selectedMenuIndex ? text.setFillColor(sf::Color::Yellow) : text.setFillColor(sf::Color::White);

			window.draw(text);
		}

		sf::Text editor;
		editor.setFont(*context.font);
		editor.setString("EDITOR");
		editor.setCharacterSize(28);
		editor.setPosition(MENU_X, MENU_START_Y + context.levelFiles->size() * MENU_SPACING);

		if (*context.selectedMenuIndex == context.levelFiles->size())
			editor.setFillColor(sf::Color::Cyan);
		else
			editor.setFillColor(sf::Color::White);

		window.draw(editor);
	}
	void MenuState::handleNavigation(GameContext& context)
	{
		const int totalOptions = context.levelFiles->size() + 1;

		if (context.input->isActionPressed("MENU_DOWN"))
		{
			(*context.selectedMenuIndex)++;

			if (*context.selectedMenuIndex >= totalOptions) *context.selectedMenuIndex = 0;

			sf::sleep(sf::milliseconds(MENU_INPUT_DELAY_MS));
		}

		if (context.input->isActionPressed("MENU_UP"))
		{
			*context.selectedMenuIndex == 0 ? *context.selectedMenuIndex = totalOptions - 1 : (*context.selectedMenuIndex)--;

			sf::sleep(sf::milliseconds(MENU_INPUT_DELAY_MS));
		}
	}
	void MenuState::selectCurrentOption(GameContext& context)
	{
		if (*context.selectedMenuIndex < context.levelFiles->size())
		{
			loadLevel(context);
		}
		else
		{
			openEditor(context);
		}
		sf::sleep(sf::milliseconds(MENU_INPUT_DELAY_MS));
	}
	void MenuState::loadLevel(GameContext& context)
	{
		*context.currentLevel = (*context.levelFiles)[*context.selectedMenuIndex];

		LevelLoader::load(
			*context.currentLevel,
			*context.tileMap,
			*context.player,
			*context.enemies,
			*context.collectibles
		);

		context.game->buildMovingPlatforms();

		context.game->changeState(std::make_unique<PlayState>());
	}
	void MenuState::openEditor(GameContext& context)
	{
		context.tileMap->clear();
		context.enemies->clear();
		context.collectibles->clear();
		context.game->changeState(std::make_unique<EditorState>());
	}
}