#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"
#include "Collectible.h"
#include "Portal.h"
#include "Game.h"
#include <memory>

namespace fp
{
    namespace
    {
        const int BRUSHES_AMOUNT = static_cast<int>(BrushType::Count);
    }

    void EditorState::render(sf::RenderWindow& window, GameContext& context)
    {
        window.setView(*context.camera);
        WorldState::render(window, context);

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
        hud.render(
            window,
            *context.camera,
            *context.font,
            brush
        );
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
    void EditorState::updateBrush(float dt, GameContext& context)
    {
        static float cooldown = 0.f;
        cooldown -= dt;

        if (cooldown > 0.f) return;

        if (context.input->isActionPressed("NEXT_BRUSH"))
        {
            brush = static_cast<BrushType>((static_cast<int>(brush) + 1) % BRUSHES_AMOUNT);
            cooldown = 0.2f;
        }

        if (context.input->isActionPressed("PREV_BRUSH"))
        {
            int value = static_cast<int>(brush) - 1;

            if (value < 0) value = BRUSHES_AMOUNT - 1;

            brush = static_cast<BrushType>(value);

            cooldown = 0.2f;
        }
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
        else if (brush == BrushType::FireFruit)
        {
            auto fireFruit = std::make_unique<FireFruit>();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float collectibleWidth = fireFruit->getGlobalBounds().width;
            const float collectibleHeight = fireFruit->getGlobalBounds().height;

            fireFruit->setPosition(
                mouseX * tileSize + (tileSize - collectibleWidth) / 2.f,
                mouseY * tileSize + (tileSize - collectibleHeight)
            );

            context.collectibles->push_back(std::move(fireFruit));
        }
        else if (brush == BrushType::Portal)
        {
            auto portal = std::make_unique<Portal>(nextPortalPairId);

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float collectibleWidth = portal->getGlobalBounds().width;
            const float collectibleHeight = portal->getGlobalBounds().height;

            portal->setPosition(
                mouseX * tileSize + (tileSize - collectibleWidth) / 2.f,
                mouseY * tileSize + (tileSize - collectibleHeight)
            );

            Portal* portalPtr = portal.get();

            context.collectibles->push_back(std::move(portal));

            if (pendingPortal == nullptr)
            {
                pendingPortal = portalPtr;
            }
            else
            {
                pendingPortal->setLinkedPortal(portalPtr);

                portalPtr->setLinkedPortal(pendingPortal);

                pendingPortal = nullptr;

                nextPortalPairId++;
            }
        }
        else if (brush == BrushType::InvulnerabilityFruit)
        {
            auto invulFruit = std::make_unique<InvulnerabilityFruit>();

            const float tileSize = static_cast<float>(context.tileMap->getTileSize());

            const float collectibleWidth = invulFruit->getGlobalBounds().width;
            const float collectibleHeight = invulFruit->getGlobalBounds().height;

            invulFruit->setPosition(
                mouseX * tileSize + (tileSize - collectibleWidth) / 2.f,
                mouseY * tileSize + (tileSize - collectibleHeight)
            );

            context.collectibles->push_back(std::move(invulFruit));
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
        else if (brush == BrushType::LifeFruit || brush == BrushType::Goal || brush == BrushType::FireFruit || brush == BrushType::Portal || brush == BrushType::InvulnerabilityFruit)
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
        Portal* linkedPortal = nullptr;

        for (auto it = context.collectibles->begin(); it != context.collectibles->end(); )
        {
            if ((*it)->getGlobalBounds().contains(pos))
            {
                if (auto portal = dynamic_cast<Portal*>(it->get()))
                {
                    linkedPortal = portal->getLinkedPortal();

                    if (portal == pendingPortal)
                    {
                        pendingPortal = nullptr;
                    }
                }

                it = context.collectibles->erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (linkedPortal)
        {
            for (auto it = context.collectibles->begin(); it != context.collectibles->end(); )
            {
                if (it->get() == linkedPortal)
                {
                    it = context.collectibles->erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
	void EditorState::handleInput(float dt, GameContext& context)
	{
		handlePlayerInput(dt, context);
		sf::RenderWindow& window = *context.window;
        placeCooldown -= dt;

        if (context.input->isActionPressed("SAVE"))
        {
            if (!context.tileMap->hasSpawn())
            {
                context.game->setTypingFileName(false);
                return;
            }
            context.game->setTypingFileName(true);
        }
        if (context.game->getTypingFileName() && context.input->isActionPressed("CANCEL"))
        {
            context.game->setTypingFileName(false);
        }
        if (context.game->getTypingFileName() && context.input->isActionPressed("CONFIRM_SAVE"))
        {
            context.game->saveLevel();

            context.game->loadLevelList();

            context.game->setTypingFileName(false);
        }
        static sf::Clock backspaceClock;

        if (context.game->getTypingFileName() &&
            context.input->isActionPressed("BACK") &&
            backspaceClock.getElapsedTime().asMilliseconds() > 150)
        {
            backspaceClock.restart();

            if (!context.game->getFileNameInput().empty())
                context.game->getFileNameInput().pop_back();
        }

        int tileX;
        int tileY;

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

        updateBrush(dt, context);

		if (context.input->isActionPressed("ADD_ELEMENT") &&
            placeCooldown <= 0.f)
		{
            addElement(context, tileX, tileY);
            placeCooldown = 0.15f;
		}
		if (context.input->isActionPressed("REMOVE_ELEMENT"))
		{
            removeElement(context, tileX, tileY);
		}
	}
}