#include "Game.h"
#include <iostream>
#include <filesystem>

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

		camera.setSize(
			static_cast<float>(WINDOW_WIDTH),
			static_cast<float>(WINDOW_HEIGHT)
		);

		camera.setCenter(
			WINDOW_WIDTH / 2.f,
			WINDOW_HEIGHT / 2.f
		);
		//fonts

		font.loadFromFile("assets/fonts/Roboto_Condensed-Black.ttf");

		fileNameText.setFont(font);
		fileNameText.setCharacterSize(24);
		fileNameText.setFillColor(sf::Color::White);
		titleMenuText.setFont(font);
		titleMenuText.setCharacterSize(40);
		titleMenuText.setFillColor(sf::Color::White);
		titleMenuText.setPosition(250.f, 80.f);

		menuText.setFont(font);
		menuText.setCharacterSize(28);
		menuText.setFillColor(sf::Color::White);

		editorHelp.setFont(font);
		editorHelp.setCharacterSize(15);
		editorHelp.setFillColor(sf::Color::White);
		editorHelp.setPosition(10.f, 10.f);
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
		const unsigned width = 224;
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
		state = GameState::Menu;
		loadLevelList();
	}

	Game::~Game()
	{
		delete player;
		delete tileMap;
	}

	void Game::updateInput()
	{
		if (typingFileName) return;
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	
		const int mouseX = static_cast<int>(worldPos.x) / tileMap->getTileSize();

		const int mouseY = static_cast<int>(worldPos.y) / tileMap->getTileSize();

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

				if (insideTileX && player->getVelocity().y >= 0.f && playerBottom <= tileBounds.top + TILE_TOP_COLLISION_OFFSET)
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

	void Game::updateCamera()
	{
		sf::FloatRect playerBounds = player->getGlobalBounds();

		float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

		float cameraCenterX = camera.getCenter().x;

		float halfWidth = camera.getSize().x / 2.f;

		// deadzone
		float rightBorder = cameraCenterX + (WINDOW_WIDTH / 2.f);

		float leftBorder = cameraCenterX - (WINDOW_WIDTH / 2.f);

		// move camera right
		if (playerCenterX > rightBorder)
		{
			cameraCenterX = playerCenterX - (WINDOW_WIDTH / 2.f);
		}

		// move camera left
		if (playerCenterX < leftBorder)
		{
			cameraCenterX = playerCenterX + (WINDOW_WIDTH / 2.f);
		}

		// map bounds
		float mapWidth = tileMap->getWidth() * tileMap->getTileSize();

		if (cameraCenterX < halfWidth) cameraCenterX = halfWidth;

		if (cameraCenterX > mapWidth - halfWidth) cameraCenterX = mapWidth - halfWidth;

		camera.setCenter(
			cameraCenterX,
			WINDOW_HEIGHT / 2.f
		);
	}

	void Game::updateMenu()
	{
		const int totalOptions = levelFiles.size() + 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			selectedMenuIndex++;

			if (selectedMenuIndex >= totalOptions) selectedMenuIndex = 0;

			sf::sleep(sf::milliseconds(150));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			selectedMenuIndex == 0 ? selectedMenuIndex = totalOptions - 1 : selectedMenuIndex--;

			sf::sleep(sf::milliseconds(150));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			// levels
			if (selectedMenuIndex < levelFiles.size())
			{
				currentLevel = levelFiles[selectedMenuIndex];

				tileMap->loadFromFile(currentLevel);

				state = GameState::Playing;
			}
			// editor
			else
			{
				state = GameState::Editor;
			}
			sf::sleep(sf::milliseconds(150));
		}
	}

	void Game::updateEditor()
	{
		updateInput();
		updatePlayer();
		updateTileCollision();
		updateCollision();
		updateCamera();
	}

	void Game::updateGameplay()
	{
		updateInput();
		updatePlayer();
		updateTileCollision();
		updateCollision();
		updateCamera();
	}

	void Game::update()
	{
		deltaTime = dtClock.restart().asSeconds();

		//poll window event
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::E)
				{
					state = GameState::Editor;
				}
				if (event.key.code == sf::Keyboard::F2)
				{
					typingFileName = true;
					fileNameInput.clear();
				}
				if (typingFileName && event.key.code == sf::Keyboard::C)
				{
					typingFileName = false;
					fileNameInput.clear();
				}
				if (typingFileName && event.key.code == sf::Keyboard::Enter)
				{
					tileMap->saveToFile("levels/" + fileNameInput + ".txt");
					typingFileName = false;
				}
				if (typingFileName && event.key.code == sf::Keyboard::BackSpace)
				{
					if (!fileNameInput.empty()) fileNameInput.pop_back();
				}
				
				if ((state == GameState::Editor || state == GameState::Playing) && event.key.code == sf::Keyboard::M)
				{
					state = GameState::Menu;
				}
			}
			if (typingFileName && event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128)
				{
					char c = static_cast<char>(event.text.unicode);
					if (c != '\b' && c != '\r')
					{
						fileNameInput += c;
					}
				}
			}
		}

		switch (state)
		{
		case GameState::Menu:
			updateMenu();
			break;

		case GameState::Editor:
			updateEditor();
			break;

		case GameState::Playing:
			updateGameplay();
			break;
		}
	}

	void Game::renderEditor()
	{
		window.setView(camera);
		renderTileMap();
		renderPlayer();
		editorHelp.setString("Press F2 to save and enter C to proceed or cancel\nPress M to open menu");
		window.draw(editorHelp);
	}

	void Game::renderMenu()
	{
		window.setView(window.getDefaultView());
		window.clear(sf::Color::Black);

		titleMenuText.setString("LEVEL SELECT");
		window.draw(titleMenuText);

		for (int i = 0; i < levelFiles.size(); i++)
		{
			std::filesystem::path path(levelFiles[i]);
			menuText.setString(path.stem().string());

			menuText.setPosition(250.f, 180.f + i * 50.f);

			i == selectedMenuIndex ? menuText.setFillColor(sf::Color::Yellow) : menuText.setFillColor(sf::Color::White);

			window.draw(menuText);
		}

		menuText.setString("EDITOR");
		menuText.setPosition(
			250.f,
			180.f + levelFiles.size() * 50.f
		);

		if (selectedMenuIndex == levelFiles.size())
			menuText.setFillColor(sf::Color::Cyan);
		else
			menuText.setFillColor(sf::Color::White);

		window.draw(menuText);
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
		window.setView(camera);

		switch (state)
		{
		case GameState::Menu:
			renderMenu();
			break;

		case GameState::Editor:
			renderEditor();
			break;

		case GameState::Playing:
			window.setView(camera);
			renderTileMap();
			renderPlayer();
			break;
		}

		if (typingFileName)
		{
			fileNameText.setString("Save as: " + fileNameInput);

			fileNameText.setPosition(
				camera.getCenter().x - 300.f,
				camera.getCenter().y
			);
			window.draw(fileNameText);
		}

		window.display();
	}

	void Game::loadLevelList()
	{
		levelFiles.clear();

		for (const auto& entry : std::filesystem::directory_iterator("levels"))
		{
			if (entry.path().extension() == ".txt")
			{
				levelFiles.push_back(entry.path().string());
			}
		}
	}

	const sf::RenderWindow& Game::getWindow() const
	{
		return window;
	}
}