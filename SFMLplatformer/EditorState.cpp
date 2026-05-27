#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    void EditorState::onWorldUpdate(float dt, GameContext& context)
    {
        sf::RenderWindow& window = *context.window;

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        int mouseX =
            static_cast<int>(worldPos.x) /
            context.tileMap->getTileSize();

        int mouseY =
            static_cast<int>(worldPos.y) /
            context.tileMap->getTileSize();

        if (context.input->isMouseDown("ADD_TILE"))
        {
            context.tileMap->addTile(mouseX, mouseY);
        }

        if (context.input->isMouseDown("REMOVE_TILE"))
        {
            context.tileMap->removeTile(mouseX, mouseY);
        }
    }

    void EditorState::render(sf::RenderWindow& window, GameContext& context)
    {
        window.setView(*context.camera);
        WorldState::render(window, context);

        sf::Text editorHelp;
        editorHelp.setFont(*context.font);
        editorHelp.setCharacterSize(15);
        editorHelp.setFillColor(sf::Color::White);
        editorHelp.setPosition(10.f, 10.f);

        editorHelp.setString("Press F2 to save and enter C to proceed or cancel\nPress M to open menu");
        window.draw(editorHelp);
    }
	void EditorState::handleEditorInput(GameContext& context)
	{
		sf::RenderWindow& window = *context.window;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

		TileMap* tileMap = context.tileMap;

		//tile func
		if (context.input->isMouseDown("ADD_TILE"))
		{
			tileMap->addTile(mouseX, mouseY);
		}
		if (context.input->isMouseDown("REMOVE_TILE"))
		{
			tileMap->removeTile(mouseX, mouseY);
		}
	}
}