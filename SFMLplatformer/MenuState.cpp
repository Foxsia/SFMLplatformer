#include "MenuState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "GameContext.h"
#include "LevelLoader.h"
#include "TileMap.h"
#include <filesystem>

namespace fp
{
	void MenuState::update(float dt, GameContext& context)
	{
		int totalOptions = context.levelFiles->size() + 1;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			(*context.selectedMenuIndex)++;

			if (*context.selectedMenuIndex >= totalOptions) *context.selectedMenuIndex = 0;

			sf::sleep(sf::milliseconds(150));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			*context.selectedMenuIndex == 0 ? *context.selectedMenuIndex = totalOptions - 1 : (*context.selectedMenuIndex)--;

			sf::sleep(sf::milliseconds(150));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			// levels
			if (*context.selectedMenuIndex < context.levelFiles->size())
			{
				*context.currentLevel = (*context.levelFiles)[*context.selectedMenuIndex];

				LevelLoader::load(
					*context.currentLevel,
					*context.tileMap,
					*context.enemies
				);

				delete* context.state;
				*context.state = new PlayState();
			}
			// editor
			else
			{
				delete* context.state;
				*context.state = new EditorState();
			}
			sf::sleep(sf::milliseconds(150));
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
		title.setPosition(250.f, 80.f);
		window.draw(title);

		for (int i = 0; i < context.levelFiles->size(); i++)
		{
			std::filesystem::path path((*context.levelFiles)[i]);

			sf::Text text;
			text.setFont(*context.font);
			text.setString(path.stem().string());
			text.setCharacterSize(28);
			text.setPosition(250.f, 180.f + i * 50.f);

			i == *context.selectedMenuIndex ? text.setFillColor(sf::Color::Yellow) : text.setFillColor(sf::Color::White);

			window.draw(text);
		}

		sf::Text editor;
		editor.setFont(*context.font);
		editor.setString("EDITOR");
		editor.setCharacterSize(28);
		editor.setPosition(250.f, 180.f + context.levelFiles->size() * 50.f);

		if (*context.selectedMenuIndex == context.levelFiles->size())
			editor.setFillColor(sf::Color::Cyan);
		else
			editor.setFillColor(sf::Color::White);

		window.draw(editor);
	}
}