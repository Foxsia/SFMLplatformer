#include "Game.h"
#include <iostream>

namespace fp
{
	namespace
	{
		const unsigned WINDOW_WIDTH = 800;
		const unsigned WINDOW_HEIGHT = 640;

		const unsigned FRAME_RATE_LIMIT = 144;

		const char* WINDOW_TITLE = "Platformer";

		const float PLAYER_MOVE_LEFT = -1.f;
		const float PLAYER_MOVE_RIGHT = 1.f;

		const float PLAYER_MOVE_Y = 0.f;

		const float TILE_TOP_COLLISION_OFFSET = 20.f;
	}

	void Game::initWindow()
	{
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close | sf::Style::Titlebar);
		window.setFramerateLimit(FRAME_RATE_LIMIT);
	}

	void Game::initInput()
	{
		//mouse
		mouseMappings["BTN_ADD_TILE"] = sf::Mouse::Button::Left;
		mouseMappings["BTN_REMOVE_TILE"] = sf::Mouse::Button::Right;

		//keyboard
		keyboardMappings["KEY_MOVE_LEFT"] = sf::Keyboard::Key::A;
		keyboardMappings["KEY_MOVE_RIGHT"] = sf::Keyboard::Key::D;
		keyboardMappings["KEY_MOVE_UP"] = sf::Keyboard::Key::W;
		keyboardMappings["KEY_MOVE_DOWN"] = sf::Keyboard::Key::S;
		keyboardMappings["KEY_JUMP"] = sf::Keyboard::Key::Space;
	}

	void Game::initTileSheet()
	{
		if (!tileSheet.loadFromFile("assets/tile_sheet.png"))
		{
			std::cout << "Error::Game::Couldn't load the tile sheet!\n";
		}
	}

	void Game::initPlayer()
	{
		player = new Player();
	}

	void Game::initTileMap()
	{
		const unsigned width = window.getSize().x / Tile::getSize();
		const unsigned height = window.getSize().y / Tile::getSize();

		tileMap = new TileMap(width, height, &tileSheet, Tile::getSize());
	}

	Game::Game()
		: deltaTime(0.f)
	{
		initWindow();
		initInput();
		initTileSheet();
		initPlayer();
		initTileMap();
	}

	Game::~Game()
	{
		delete player;
		delete tileMap;
	}

	void Game::updateInput()
	{
		//update mouse pos
		const int mouseX = int(sf::Mouse::getPosition(getWindow()).x / int(tileMap->getTileSize()));
		const int mouseY = int(sf::Mouse::getPosition(getWindow()).y / int(tileMap->getTileSize()));

		//player movement
		if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_MOVE_LEFT"]))
		{
			player->move(PLAYER_MOVE_LEFT, PLAYER_MOVE_Y, deltaTime);
		}
		else if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_MOVE_RIGHT"]))
		{
			player->move(PLAYER_MOVE_RIGHT, PLAYER_MOVE_Y, deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_JUMP"]) && player->getCanJump())
		{
			player->jump();
		}

		//tile func
		if (sf::Mouse::isButtonPressed(mouseMappings["BTN_ADD_TILE"]))
		{
			tileMap->addTile(mouseX, mouseY);
		}
		else if (sf::Mouse::isButtonPressed(mouseMappings["BTN_REMOVE_TILE"]))
		{
			tileMap->removeTile(mouseX, mouseY);
		}
	}

	void Game::updatePlayer()
	{
		player->update(deltaTime);
	}

	void Game::updateCollision()
	{
		if (player->getPosition().y + player->getGlobalBounds().height > window.getSize().y)
		{
			player->setCanJump(true);
			player->resetVelocityY();
			player->setPosition(player->getPosition().x, window.getSize().y - player->getGlobalBounds().height);
		}
	}

	void Game::updateTileCollision()
	{
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

				if (insideTileX && playerBottom <= tileBounds.top + TILE_TOP_COLLISION_OFFSET)
				{
					player->setPosition(
						playerBounds.left,
						tileBounds.top - playerBounds.height
					);

					player->resetVelocityY();

					grounded = true;

					playerBounds = player->getGlobalBounds();
				}
			}
		}

		player->setCanJump(grounded);
	}

	void Game::update()
	{
		deltaTime = dtClock.restart().asSeconds();

		//poll window event
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
		}

		updateInput();
		updatePlayer();
		updateTileCollision();
		updateCollision();
	}

	void Game::renderPlayer()
	{
		player->render(window);
	}

	void Game::renderTileMap()
	{
		tileMap->render(window);
	}

	void Game::render()
	{
		window.clear();

		renderTileMap();
		renderPlayer();

		window.display();
	}

	const sf::RenderWindow& Game::getWindow() const
	{
		return window;
	}
}