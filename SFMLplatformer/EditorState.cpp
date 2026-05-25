#include "EditorState.h"
#include "GameContext.h"
#include "TileMap.h"
#include "Player.h"

namespace fp
{
    void EditorState::update(float dt, GameContext& context)
    {
        updateInput(dt, context);
		context.player->update(dt);
        updateTileCollision(context);
        updateCollision(context);
        updateCamera(context);
    }

    void EditorState::render(sf::RenderWindow& window, GameContext& context)
    {
        window.setView(*context.camera);
        context.tileMap->render(window);
        context.player->render(window);

        sf::Text editorHelp;
        editorHelp.setFont(*context.font);
        editorHelp.setCharacterSize(15);
        editorHelp.setFillColor(sf::Color::White);
        editorHelp.setPosition(10.f, 10.f);

        editorHelp.setString("Press F2 to save and enter C to proceed or cancel\nPress M to open menu");
        window.draw(editorHelp);
    }
    void EditorState::updateInput(float dt, GameContext& context)
    {
		sf::RenderWindow& window = *context.window;

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		const int mouseX = static_cast<int>(worldPos.x) / context.tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / context.tileMap->getTileSize();

		Player* player = context.player;
		TileMap* tileMap = context.tileMap;

		auto& keys = *context.keyboardMappings;
		auto& mouse = *context.mouseMappings;
		//player movement
		if (sf::Keyboard::isKeyPressed(keys["KEY_MOVE_LEFT"]))
		{
			player->move(-1.f, 0.f, dt);
		}
		else if (sf::Keyboard::isKeyPressed(keys["KEY_MOVE_RIGHT"]))
		{
			player->move(1.f, 0.f, dt);
		}

		if (sf::Keyboard::isKeyPressed(keys["KEY_JUMP"]) && player->getCanJump())
		{
			player->jump();
		}

		//tile func
		if (sf::Mouse::isButtonPressed(mouse["BTN_ADD_TILE"]))
		{
			tileMap->addTile(mouseX, mouseY);
		}
		else if (sf::Mouse::isButtonPressed(mouse["BTN_REMOVE_TILE"]))
		{
			tileMap->removeTile(mouseX, mouseY);
		}
    }
	void EditorState::updateTileCollision(GameContext& context)
	{
		Player* player = context.player;
		TileMap* tileMap = context.tileMap;

		bool grounded = false;

		sf::FloatRect playerBounds = player->getGlobalBounds();

		for (int x = 0; x < tileMap->getWidth(); x++)
		{
			for (int y = 0; y < tileMap->getHeight(); y++)
			{
				Tile* tile = tileMap->getTile(x, y);

				if (!tile) continue;

				sf::FloatRect tileBounds = tile->getHitbox();

				if (!playerBounds.intersects(tileBounds)) continue;

				// collision only from top
				float playerBottom = playerBounds.top + playerBounds.height;
				float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

				bool insideTileX =
					playerCenterX > tileBounds.left &&
					playerCenterX < tileBounds.left + tileBounds.width;

				if (insideTileX && player->getVelocity().y >= 0.f && playerBottom <= tileBounds.top + 20.f)
				{
					player->setPosition(
						playerBounds.left,
						tileBounds.top - playerBounds.height
					);

					player->resetVelocityY();

					grounded = true;
				}
			}
		}

		player->setCanJump(grounded);
	}
	void EditorState::updateCollision(GameContext& context)
	{
		if (context.player->getPosition().y + context.player->getGlobalBounds().height > context.window->getSize().y)
		{
			context.player->setCanJump(true);
			context.player->resetVelocityY();
			context.player->setPosition(context.player->getPosition().x, context.window->getSize().y - context.player->getGlobalBounds().height);
		}
	}
	void EditorState::updateCamera(GameContext& context)
	{
		sf::FloatRect playerBounds = context.player->getGlobalBounds();

		float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

		float cameraCenterX = context.camera->getCenter().x;

		float halfWidth = context.camera->getSize().x / 2.f;

		// deadzone
		float rightBorder = cameraCenterX + (context.camera->getSize().x / 2.f);

		float leftBorder = cameraCenterX - (context.camera->getSize().x / 2.f);

		// move camera right
		if (playerCenterX > rightBorder)
		{
			cameraCenterX = playerCenterX - (context.camera->getSize().x / 2.f);
		}

		// move camera left
		if (playerCenterX < leftBorder)
		{
			cameraCenterX = playerCenterX + (context.camera->getSize().x / 2.f);
		}

		// map bounds
		float mapWidth = context.tileMap->getWidth() * context.tileMap->getTileSize();

		if (cameraCenterX < halfWidth) cameraCenterX = halfWidth;

		if (cameraCenterX > mapWidth - halfWidth) cameraCenterX = mapWidth - halfWidth;

		context.camera->setCenter(
			cameraCenterX,
			context.camera->getSize().y / 2.f
		);
	}
}