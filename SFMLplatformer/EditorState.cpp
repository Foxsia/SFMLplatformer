#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    namespace
    {
        const unsigned HELP_TEXT_SIZE = 15;
        const float HELP_TEXT_X = 10.f;
        const float HELP_TEXT_Y = 10.f;
    }

    void EditorState::render(sf::RenderWindow& window, GameContext& context)
    {
        window.setView(*context.camera);
        WorldState::render(window, context);

        sf::Text editorHelp;
        editorHelp.setFont(*context.font);
        editorHelp.setCharacterSize(HELP_TEXT_SIZE);
        editorHelp.setFillColor(sf::Color::White);
        editorHelp.setPosition(HELP_TEXT_X, HELP_TEXT_Y);

        editorHelp.setString("Press F2 to save and Enter to proceed or C to cancel\nPress M to open menu");
        window.draw(editorHelp);
    }
    void EditorState::updateBrush(GameContext& context)
    {
        if (context.input->isKeyDown("TILE_BRUSH")) brush = BrushType::Tile;

        if (context.input->isKeyDown("ENEMY_BRUSH")) brush = BrushType::Enemy;
    }
    void EditorState::addElement(GameContext& context, int mouseX, int mouseY)
    {
        if (brush == BrushType::Tile)
        {
            context.tileMap->addTile(mouseX, mouseY);
        }
        else if (brush == BrushType::Enemy)
        {
            Enemy* enemy = new Enemy();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float enemyWidth = enemy->getGlobalBounds().width;
            const float enemyHeight = enemy->getGlobalBounds().height;

            enemy->setPosition(
                mouseX * tileSize + (tileSize - enemyWidth) / 2.f,
                mouseY * tileSize + (tileSize - enemyHeight)
            );

            context.enemies->push_back(enemy);
        }
    }
    void EditorState::removeElement(GameContext& context, int mouseX, int mouseY)
    {
        sf::RenderWindow& window = *context.window;

        if (brush == BrushType::Tile)
        {
            context.tileMap->removeTile(mouseX, mouseY);
        }
        else
        {
            removeEnemyAtPosition(context, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        }
    }
    void EditorState::removeEnemyAtPosition(GameContext& context, const sf::Vector2f& pos)
    {
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
	void EditorState::handleInput(float dt, GameContext& context)
	{
		handlePlayerInput(dt, context);
		sf::RenderWindow& window = *context.window;

		const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

        updateBrush(context);

		if (context.input->isMouseDown("ADD_ELEMENT"))
		{
            addElement(context, mouseX, mouseY);
		}
		if (context.input->isMouseDown("REMOVE_ELEMENT"))
		{
            removeElement(context, mouseX, mouseY);
		}
	}
}