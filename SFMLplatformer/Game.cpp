#include "Game.h"
#include "MenuState.h"
#include "EditorState.h"
#include "GameContext.h"
#include "LevelLoader.h"
#include "Utils.h"
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

		const float JOYSTICK_THRESHOLD_LEFT = -50.f;
		const float JOYSTICK_THRESHOLD_RIGHT = 50.f;

		const float JOYSTICK_THRESHOLD_DOWN = -50.f;
		const float JOYSTICK_THRESHOLD_UP = 50.f;

		const float LEVEL_DURATION = 300.f;
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

		font.loadFromFile("assets/fonts/Roboto_Condensed-Black.ttf");

		fileNameText.setFont(font);
		fileNameText.setCharacterSize(24);
		fileNameText.setFillColor(sf::Color::White);
	}

	void Game::initInput()
	{
		input = std::make_unique<InputManager>();

		input->bindKey("MOVE_LEFT", sf::Keyboard::A);
		input->bindKey("MOVE_RIGHT", sf::Keyboard::D);
		input->bindKey("JUMP", sf::Keyboard::Space);

		input->bindKey("PLAYER_BRUSH", sf::Keyboard::Num1);
		input->bindKey("TILE_BRUSH", sf::Keyboard::Num2);
		input->bindKey("ENEMY_BRUSH", sf::Keyboard::Num3);
		input->bindKey("LIFE_BRUSH", sf::Keyboard::Num4);
		input->bindKey("MOVING_TILE_BRUSH", sf::Keyboard::Num5);
		input->bindKey("GOAL_BRUSH", sf::Keyboard::Num6);
		input->bindKey("FIRE_BRUSH", sf::Keyboard::Num7);

		input->bindMouse("ADD_ELEMENT", sf::Mouse::Left);
		input->bindMouse("REMOVE_ELEMENT", sf::Mouse::Right);

		input->bindGamepadButton("JUMP", 0);
		input->bindGamepadButton("PLAYER_BRUSH", 0);
		input->bindGamepadButton("TILE_BRUSH", 2);
		input->bindGamepadButton("ENEMY_BRUSH", 3);
		input->bindGamepadButton("LIFE_BRUSH", 4);
		input->bindGamepadButton("MOVING_TILE_BRUSH", 5);
		input->bindGamepadButton("GOAL_BRUSH", 8);

		input->bindGamepadButton("ADD_ELEMENT", 7); 
		input->bindGamepadButton("REMOVE_ELEMENT", 6);

		input->bindJoystickAxis("MOVE_RIGHT",sf::Joystick::X, JOYSTICK_THRESHOLD_RIGHT);
		input->bindJoystickAxis("MOVE_LEFT",sf::Joystick::X, JOYSTICK_THRESHOLD_LEFT);

		input->bindJoystickAxis("MOVE_CURSOR_LEFT",sf::Joystick::Z, JOYSTICK_THRESHOLD_LEFT);
		input->bindJoystickAxis("MOVE_CURSOR_RIGHT",sf::Joystick::Z, JOYSTICK_THRESHOLD_RIGHT);
		input->bindJoystickAxis("MOVE_CURSOR_UP", sf::Joystick::R, JOYSTICK_THRESHOLD_UP);
		input->bindJoystickAxis("MOVE_CURSOR_DOWN", sf::Joystick::R, JOYSTICK_THRESHOLD_DOWN);

		input->bindKey("MENU_UP", sf::Keyboard::Up);
		input->bindKey("MENU_DOWN", sf::Keyboard::Down);
		input->bindKey("MENU_SELECT", sf::Keyboard::Enter);
		input->bindKey("MENU_BACK", sf::Keyboard::M);

		input->bindKey("SAVE", sf::Keyboard::F2);
		input->bindKey("CANCEL", sf::Keyboard::C);
		input->bindKey("BACK", sf::Keyboard::BackSpace);
		input->bindKey("CONFIRM_SAVE", sf::Keyboard::Enter);

		input->bindGamepadButton("SAVE", 10);
		input->bindGamepadButton("CANCEL", 11);
		input->bindGamepadButton("BACK", 9);
		input->bindGamepadButton("CONFIRM_SAVE", 0);

		input->bindJoystickAxis("MENU_UP", sf::Joystick::PovY, JOYSTICK_THRESHOLD_RIGHT);
		input->bindJoystickAxis("MENU_DOWN", sf::Joystick::PovY, JOYSTICK_THRESHOLD_LEFT);

		input->bindJoystickAxis("MENU_UP", sf::Joystick::Y, JOYSTICK_THRESHOLD_LEFT);
		input->bindJoystickAxis("MENU_DOWN", sf::Joystick::Y, JOYSTICK_THRESHOLD_RIGHT);

		input->bindGamepadButton("MENU_SELECT", 0);

		input->bindGamepadButton("MENU_BACK", 1);

		input->bindKey("SHOOT", sf::Keyboard::F);
		input->bindGamepadButton("SHOOT", 2);
	}

	void Game::initTileSheet()
	{
		if (!tileSheet.loadFromFile("assets/world_tileset.png"))
		{
			std::cout << "Error::Game::Couldn't load the tile sheet!\n";
		}
	}

	void Game::initPlayer()
	{
		player = std::make_unique<Player>();
	}

	void Game::initTileMap()
	{
		const unsigned width = 224;
		const unsigned height = window.getSize().y / Tile::getSize();

		tileMap = std::make_unique<TileMap>(width, height, &tileSheet, Tile::getSize());
		tileMap->loadBackground("assets/background.png");
	}

	void Game::buildMovingPlatforms()
	{
		movingPlatforms.clear();

		auto platform = std::make_unique<MovingPlatform>();

		for (unsigned x = 0; x < tileMap->getWidth(); x++)
		{
			for (unsigned y = 0; y < tileMap->getHeight(); y++)
			{
				Tile* tile = tileMap->getTile(x, y);

				if (tile && tile->getType() == TileType::Moving)
				{
					platform->addTile(tile);
				}
			}
		}

		if (!platform->getTiles().empty())
		{
			movingPlatforms.push_back(std::move(platform));
		}
	}

	void Game::restartLevelTimer()
	{
		levelTimer.restart();
	}

	Game::Game()
		: deltaTime(0.f), levelDuration(LEVEL_DURATION)
	{
		initWindow();
		initInput();
		initTileSheet();
		initPlayer();
		initTileMap();
		stateManager.changeState(StateType::Menu);
		loadLevelList();
	}

	void Game::updatePlayer()
	{
		player->update(deltaTime);
	}

	void Game::update()
	{
		deltaTime = dtClock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

			if (input.get()->isActionPressed("SAVE"))
			{
				if (!tileMap->hasSpawn())
				{
					typingFileName = false;
					return;
				}
				typingFileName = true;
				fileNameInput.clear();
			}
			if (typingFileName && input.get()->isActionPressed("CANCEL"))
			{
				typingFileName = false;
				fileNameInput.clear();
			}
			if (typingFileName && input.get()->isActionPressed("CONFIRM_SAVE"))
			{
				tileMap->saveToFile("levels/" + fileNameInput + ".txt", toRaw(enemies), toRaw(collectibles));

				loadLevelList();

				typingFileName = false;
			}
			static sf::Clock backspaceClock;

			if (typingFileName &&
				input->isActionPressed("BACK") &&
				backspaceClock.getElapsedTime().asMilliseconds() > 150)
			{
				backspaceClock.restart();

				if (!fileNameInput.empty())
					fileNameInput.pop_back();
			}

			if (input->isActionPressed("MENU_BACK"))
			{
				tileMap->clear();

				enemies.clear();

				collectibles.clear();

				stateManager.changeState(StateType::Menu);
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

		fp::GameContext context;

		context.game = this;
		context.window = &window;
		context.font = &font;

		context.tileMap = tileMap.get();
		context.platforms = &movingPlatforms;
		context.player = player.get();
		context.enemies = &enemies;
		context.collectibles = &collectibles;
		context.fireballs = &fireballs;

		context.selectedMenuIndex = &selectedMenuIndex;
		context.levelFiles = &levelFiles;

		context.currentLevel = &currentLevel;
		context.stateManager = &stateManager;
		context.isEditor = dynamic_cast<EditorState*>(stateManager.getCurrentState()) != nullptr;
		context.camera = &camera;

		context.input = input.get();

		context.levelTime = levelDuration - levelTimer.getElapsedTime().asSeconds();

		if (auto* state = stateManager.getCurrentState())
		{
			state->update(deltaTime, context);
		}
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

		fp::GameContext context;

		context.window = &window;
		context.font = &font;

		context.tileMap = tileMap.get();
		context.player = player.get();
		context.enemies = &enemies;
		context.collectibles = &collectibles;
		context.fireballs = &fireballs;

		context.selectedMenuIndex = &selectedMenuIndex;
		context.levelFiles = &levelFiles;

		context.currentLevel = &currentLevel;
		context.stateManager = &stateManager;
		context.isEditor = dynamic_cast<EditorState*>(stateManager.getCurrentState()) != nullptr;
		context.camera = &camera;

		context.input = input.get();

		context.levelTime = levelDuration - levelTimer.getElapsedTime().asSeconds();

		if (auto* state = stateManager.getCurrentState())
		{
			state->render(window, context);
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