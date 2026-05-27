#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{

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
	void EditorState::handleInput(float dt, GameContext& context)
	{
        context.isEditor = true;
		handlePlayerInput(dt, context);
		sf::RenderWindow& window = *context.window;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

		TileMap* tileMap = context.tileMap;
        if (context.input->isKeyDown("TILE_BRUSH")) brush = BrushType::Tile;

        if (context.input->isKeyDown("ENEMY_BRUSH")) brush = BrushType::Enemy;

		//tile func
		if (context.input->isMouseDown("ADD_ELEMENT"))
		{
            if (brush == BrushType::Tile)
            {
                context.tileMap->addTile(mouseX, mouseY);
            }
            else if (brush == BrushType::Enemy)
            {
                Enemy* enemy = new Enemy();
                enemy->setPosition(
                    mouseX * context.tileMap->getTileSize(),
                    mouseY * context.tileMap->getTileSize()
                );

                context.enemies->push_back(enemy);
            }
		}
		if (context.input->isMouseDown("REMOVE_ELEMENT"))
		{
            if (brush == BrushType::Tile)
            {
                context.tileMap->removeTile(mouseX, mouseY);
            }
            else
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                for (auto it = context.enemies->begin(); it != context.enemies->end(); )
                {
                    if ((*it)->getGlobalBounds().contains(pos))
                    {
                        delete* it;
                        it = context.enemies->erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
		}
	}
}