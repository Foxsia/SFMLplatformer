#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "Collectible.h"
#include "Game.h"
#include <memory>

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

        editorHelp.setString("Press F2 to save and Enter to proceed or C to cancel\nPress M to open menu\nPLAYER SPAWN POINT IS REQUIRED!");
        window.draw(editorHelp);

        if (context.tileMap->hasSpawn())
        {
            sf::RectangleShape marker;

            const float size = static_cast<float>(context.tileMap->getTileSize());

            marker.setSize({ size, size });

            marker.setPosition(context.tileMap->getPlayerSpawn());

            marker.setFillColor(sf::Color(0, 255, 0, 150));

            window.draw(marker);
        }

        if (sf::Joystick::isConnected(0))
        {
            sf::RectangleShape selection;

            float size = static_cast<float>(context.tileMap->getTileSize());

            selection.setSize({ size, size });
            selection.setPosition(
                cursorX * size,
                cursorY * size
            );

            selection.setFillColor(sf::Color(255, 255, 0, 80));
            selection.setOutlineThickness(2.f);
            selection.setOutlineColor(sf::Color::Yellow);

            window.draw(selection);
        }
    }
    void EditorState::updateGamepadCursor(float dt, GameContext& context)
    {
        moveCooldown -= dt;

        if (moveCooldown > 0.f) return;

        bool moved = false;

        if (context.input->isActionPressed("MOVE_CURSOR_RIGHT"))
        {
            cursorX++;
            moved = true;
        }
        else if (context.input->isActionPressed("MOVE_CURSOR_LEFT"))
        {
            cursorX--;
            moved = true;
        }

        if (context.input->isActionPressed("MOVE_CURSOR_UP"))
        {
            cursorY++;
            moved = true;
        }
        else if (context.input->isActionPressed("MOVE_CURSOR_DOWN"))
        {
            cursorY--;
            moved = true;
        }

        if (moved) moveCooldown = 0.15f;
    }
    void EditorState::updateBrush(GameContext& context)
    {
        if (context.input->isActionPressed("TILE_BRUSH")) brush = BrushType::Tile;

        if (context.input->isActionPressed("ENEMY_BRUSH")) brush = BrushType::Enemy;

        if (context.input->isActionPressed("PLAYER_BRUSH")) brush = BrushType::Player;

        if (context.input->isActionPressed("COLLECTIBLE_BRUSH")) brush = BrushType::LifeFruit;

        if (context.input->isActionPressed("GOAL_BRUSH")) brush = BrushType::Goal;

        if (context.input->isActionPressed("MOVING_TILE_BRUSH")) brush = BrushType::MovingTile;
    }
    void EditorState::addElement(GameContext& context, int mouseX, int mouseY)
    {
        if (brush == BrushType::Tile)
        {
            context.tileMap->addTile(mouseX, mouseY);
        }
        else if (brush == BrushType::Enemy)
        {
            auto enemy = std::make_unique<Enemy>();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float enemyWidth = enemy->getGlobalBounds().width;
            const float enemyHeight = enemy->getGlobalBounds().height;

            enemy->setPosition(
                mouseX * tileSize + (tileSize - enemyWidth) / 2.f,
                mouseY * tileSize + (tileSize - enemyHeight)
            );

            context.enemies->push_back(std::move(enemy));
        }
        else if (brush == BrushType::Player)
        {
            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            context.tileMap->setPlayerSpawn(mouseX * tileSize, mouseY * tileSize);
        }
        else if (brush == BrushType::LifeFruit)
        {
            auto lifeFruit = std::make_unique<LifeFruit>();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float collectibleWidth = lifeFruit->getGlobalBounds().width;
            const float collectibleHeight = lifeFruit->getGlobalBounds().height;

            lifeFruit->setPosition(
                mouseX * tileSize + (tileSize - collectibleWidth) / 2.f,
                mouseY * tileSize + (tileSize - collectibleHeight)
            );

            context.collectibles->push_back(std::move(lifeFruit));
        }
        else if (brush == BrushType::MovingTile)
        {
            context.tileMap->addMovingTile(mouseX, mouseY);
        }
        else if (brush == BrushType::Goal)
        {
            auto goal = std::make_unique<Goal>();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float collectibleWidth = goal->getGlobalBounds().width;
            const float collectibleHeight = goal->getGlobalBounds().height;

            goal->setPosition(
                mouseX * tileSize + (tileSize - collectibleWidth) / 2.f,
                mouseY * tileSize + (tileSize - collectibleHeight)
            );

            context.collectibles->push_back(std::move(goal));
        }
    }
    void EditorState::removeElement(GameContext& context, int mouseX, int mouseY)
    {
        sf::RenderWindow& window = *context.window;

        if (brush == BrushType::Tile || brush == BrushType::MovingTile)
        {
            context.tileMap->removeTile(mouseX, mouseY);
        }
        else if (brush == BrushType::Enemy)
        {
            float tileSize = context.tileMap->getTileSize();

            sf::Vector2f pos(
                mouseX * tileSize + tileSize / 2,
                mouseY * tileSize + tileSize / 2
            );
            removeEnemyAtPosition(context, pos);
        }
        else if (brush == BrushType::LifeFruit || brush == BrushType::Goal)
        {
            float tileSize = context.tileMap->getTileSize();

            sf::Vector2f pos(
                mouseX * tileSize + tileSize / 2,
                mouseY * tileSize + tileSize / 2
            );
            removeCollectibleAtPosition(context, pos);
        }
        else if (brush == BrushType::Player)
        {
            if (context.tileMap->hasSpawn())
            {
                sf::FloatRect spawnRect(context.tileMap->getPlayerSpawn().x, context.tileMap->getPlayerSpawn().y, context.tileMap->getTileSize(), context.tileMap->getTileSize());

                if (spawnRect.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
                {
                    context.tileMap->removePlayerSpawn();
                }
            }
        }
    }
    void EditorState::removeEnemyAtPosition(GameContext& context, const sf::Vector2f& pos)
    {
        for (auto it = context.enemies->begin(); it != context.enemies->end(); )
        {
            if ((*it)->getGlobalBounds().contains(pos))
            {
                it = context.enemies->erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    void EditorState::removeCollectibleAtPosition(GameContext& context, const sf::Vector2f& pos)
    {
        for (auto it = context.collectibles->begin(); it != context.collectibles->end(); )
        {
            if ((*it)->getGlobalBounds().contains(pos))
            {
                it = context.collectibles->erase(it);
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

        int tileX;
        int tileY;
        for (unsigned i = 0; i < sf::Joystick::getButtonCount(0); i++)
        {
            if (sf::Joystick::isButtonPressed(0, i))
            {
                std::cout << "BUTTON: " << i << "\n";
            }
        }

        if (sf::Joystick::isConnected(0))
        {
            updateGamepadCursor(dt, context);

            tileX = cursorX;
            tileY = cursorY;
        }
        else
        {
            const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

            const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            tileX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

            tileY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();
        }

        updateBrush(context);

		if (context.input->isActionPressed("ADD_ELEMENT"))
		{
            addElement(context, tileX, tileY);
		}
		if (context.input->isActionPressed("REMOVE_ELEMENT"))
		{
            removeElement(context, tileX, tileY);
		}
	}
}