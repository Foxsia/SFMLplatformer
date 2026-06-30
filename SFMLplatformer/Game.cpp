#include "Game.h"
#include "MenuState.h"
#include "EditorState.h"
#include "GameContext.h"
#include "LevelLoader.h"
#include "EntityFactory.h"
#include "Utils.h"
#include <iostream>
#include <filesystem>
#include "TileCreator.h"
#include "EnemyCreator.h"
#include "LifeFruitCreator.h"
#include "MovingTileCreator.h"
#include "GoalCreator.h"
#include "FireFruitCreator.h"
#include "InvulnerabilityFruitCreator.h"
#include "FireEnemyCreator.h"
#include <sstream>
#include <fstream>
#include "SpikeCreator.h"
#include "CollisionSystem.h"

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

		debugText.setFont(font);
		debugText.setCharacterSize(20);
		debugText.setFillColor(sf::Color::White);

		debugResultText.setFont(font);
		debugResultText.setCharacterSize(20);
		debugResultText.setFillColor(sf::Color::Red);

		teleportResultText.setFont(font);
		teleportResultText.setCharacterSize(20);
		teleportResultText.setFillColor(sf::Color::Red);
	}

	void Game::initInput()
	{
		input = std::make_unique<InputManager>();

		input->bindKey("MOVE_LEFT", sf::Keyboard::A);
		input->bindKey("MOVE_RIGHT", sf::Keyboard::D);
		input->bindKey("JUMP", sf::Keyboard::Space);

		input->bindMouse("ADD_ELEMENT", sf::Mouse::Left);
		input->bindMouse("REMOVE_ELEMENT", sf::Mouse::Right);

		input->bindGamepadButton("JUMP", 0);

		input->bindGamepadButton("PREV_BRUSH", 4);
		input->bindGamepadButton("NEXT_BRUSH", 5);

		input->bindKey("PREV_BRUSH", sf::Keyboard::Q);
		input->bindKey("NEXT_BRUSH", sf::Keyboard::E);

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
		input->bindGamepadButton("CANCEL", 2);
		input->bindGamepadButton("BACK", 3);
		input->bindGamepadButton("CONFIRM_SAVE", 11);

		input->bindJoystickAxis("MENU_UP", sf::Joystick::PovY, JOYSTICK_THRESHOLD_RIGHT);
		input->bindJoystickAxis("MENU_DOWN", sf::Joystick::PovY, JOYSTICK_THRESHOLD_LEFT);

		input->bindJoystickAxis("MENU_UP", sf::Joystick::Y, JOYSTICK_THRESHOLD_LEFT);
		input->bindJoystickAxis("MENU_DOWN", sf::Joystick::Y, JOYSTICK_THRESHOLD_RIGHT);

		input->bindGamepadButton("MENU_SELECT", 0);

		input->bindGamepadButton("MENU_BACK", 1);

		input->bindKey("SHOOT", sf::Keyboard::F);
		input->bindGamepadButton("SHOOT", 2);

		input->bindGamepadButton("DEBUG_MENU", 8);
		input->bindKey("DEBUG_MENU", sf::Keyboard::F1);

		input->bindKey("DEBUG_KILL_ENEMIES", sf::Keyboard::F7);
		input->bindKey("DEBUG_REFILL_HEALTH", sf::Keyboard::F8);
		input->bindKey("DEBUG_TELEPORT_END", sf::Keyboard::F9);
		input->bindKey("DEBUG_TELEPORT", sf::Keyboard::F10);

		input->bindKey("SUBMIT_SCORE", sf::Keyboard::Tab);
		input->bindGamepadButton("SUBMIT_SCORE", 5);
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

	void Game::initCreators()
	{
		EntityFactory::registerCreator(
			1,
			std::make_unique<TileCreator>());

		EntityFactory::registerCreator(
			2,
			std::make_unique<EnemyCreator>());

		EntityFactory::registerCreator(
			3,
			std::make_unique<LifeFruitCreator>());

		EntityFactory::registerCreator(
			4,
			std::make_unique<MovingTileCreator>());

		EntityFactory::registerCreator(
			5,
			std::make_unique<GoalCreator>());

		EntityFactory::registerCreator(
			6,
			std::make_unique<FireFruitCreator>());

		EntityFactory::registerCreator(
			7,
			std::make_unique<InvulnerabilityFruitCreator>());
		EntityFactory::registerCreator(
			8,
			std::make_unique<FireEnemyCreator>());
		EntityFactory::registerCreator(
			9,
			std::make_unique<SpikeCreator>());
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

	void Game::setTypingFileName(bool isTyping)
	{
		typingFileName = isTyping;
		fileNameInput.clear();
	}

	void Game::setShowDebug(bool show)
	{
		showDebugMenu = show;
	}

	void Game::saveLevel()
	{
		tileMap->saveToFile("levels/" + fileNameInput + ".txt",
			toRaw(enemies),
			toRaw(collectibles));
	}

	Game::Game()
		: deltaTime(0.f), levelDuration(LEVEL_DURATION)
	{
		initWindow();
		initInput();
		initCreators();
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
			if (auto* state = stateManager.getCurrentState())
			{
				state->handleEvent(event);
			}

			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();

			if (input->isActionPressed("MENU_BACK") && !typingFileName)
			{
				if (auto* state = stateManager.getCurrentState())
				{
					if (state->blocksGameInput()) return;
				}

				tileMap->clear();

				enemies.clear();

				collectibles.clear();

				showDebugMenu = false;

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

			if (typingTeleport && event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128)
				{
					char c = static_cast<char>(event.text.unicode);

					if (c == '\b')
					{
						if (!teleportInput.empty())
							teleportInput.pop_back();
					}
					else if (c != '\r')
					{
						teleportInput += c;
					}
				}
			}

			if (input->isActionPressed("DEBUG_MENU")) showDebugMenu = !showDebugMenu;
		}

		if (showDebugMenu)
		{
			if (input->isActionPressed("DEBUG_KILL_ENEMIES")) enemies.clear();

			if (input->isActionPressed("DEBUG_REFILL_HEALTH")) player->heal();

			if (input->isActionPressed("DEBUG_TELEPORT_END"))
			{
				for (auto& collectible : collectibles)
				{
					if (auto* goal = dynamic_cast<Goal*>(collectible.get()))
					{
						auto pos = goal->getPosition();

						player->setPosition(
							pos.x - 3 * tileMap->getTileSize(),
							pos.y - 2 * tileMap->getTileSize()
						);
						player->resetVelocityY();
						player->resetVelocityX();
						break;
					}
				}
			}

			if (input->isActionPressed("DEBUG_TELEPORT"))
			{
				typingTeleport = true;
				teleportInput.clear();
			}

			if (typingTeleport && input->isActionPressed("CONFIRM_SAVE"))
			{
				std::stringstream ss(teleportInput);

				ss >> teleportX >> teleportY;

				sf::FloatRect targetBounds(teleportX, teleportY, player->getHitbox().width, player->getHitbox().height);

				if (CollisionSystem::canTeleportTo(targetBounds, *tileMap, enemies))
				{
					player->setPosition(teleportX, teleportY);

					player->resetVelocityX();
					player->resetVelocityY();
					showTeleportError = false;
				}
				else
				{
					showTeleportError = true;
					teleportErrorClock.restart();
				}

				typingTeleport = false;
			}
		}


		if (showTeleportError &&
			teleportErrorClock.getElapsedTime().asSeconds() > 2.f)
		{
			showTeleportError = false;
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

		if (typingTeleport)
		{
			fileNameText.setString("X Y: " + teleportInput);

			fileNameText.setPosition(
				camera.getCenter().x - 300.f,
				camera.getCenter().y
			);
			window.draw(fileNameText);
		}

		if (showTeleportError)
		{
			teleportResultText.setString("Cannot teleport there!");
			teleportResultText.setPosition(
				camera.getCenter().x - 300.f,
				camera.getCenter().y + 40.f
			);

			window.draw(teleportResultText);
		}

		if (showDebugMenu)
		{
			debugText.setString(
				"DEBUG MENU\n\n"
				"[F7] Kill All Enemies\n"
				"[F8] Refill Health\n"
				"[F9] Teleport to end\n"
				"[F10] Teleport to XY\n"
			);

			debugText.setPosition(
				camera.getCenter().x - camera.getSize().x / 2.f + 20.f,
				camera.getCenter().y - 200.f
			);

			window.draw(debugText);

			sf::RectangleShape hitbox;

			auto hitBoxbounds = player->getHitbox();

			hitbox.setPosition(hitBoxbounds.left, hitBoxbounds.top);
			hitbox.setSize({ hitBoxbounds.width, hitBoxbounds.height });

			hitbox.setFillColor(sf::Color::Transparent);
			hitbox.setOutlineColor(sf::Color::Red);
			hitbox.setOutlineThickness(2.f);

			window.draw(hitbox);

			sf::RectangleShape boundsRect;

			auto bounds = player->getGlobalBounds();

			boundsRect.setPosition(bounds.left, bounds.top);
			boundsRect.setSize({ bounds.width, bounds.height });

			boundsRect.setFillColor(sf::Color::Transparent);
			boundsRect.setOutlineColor(sf::Color::Blue);
			boundsRect.setOutlineThickness(2.f);

			window.draw(boundsRect);

			for (unsigned x = 0; x < tileMap->getWidth(); x++)
			{
				for (unsigned y = 0; y < tileMap->getHeight(); y++)
				{
					Tile* tile = tileMap->getTile(x, y);

					if (!tile) continue;

					auto hitbox = tile->getHitbox();

					sf::RectangleShape boxHitbox;
					boxHitbox.setPosition(hitbox.left, hitbox.top);
					boxHitbox.setSize({ hitbox.width, hitbox.height });

					boxHitbox.setFillColor(sf::Color::Transparent);
					boxHitbox.setOutlineColor(sf::Color::Green);
					boxHitbox.setOutlineThickness(1.f);

					window.draw(boxHitbox);

					auto bounds = tile->getGlobalBounds();

					sf::RectangleShape boxBounds;
					boxBounds.setPosition(bounds.left, bounds.top);
					boxBounds.setSize({ bounds.width, bounds.height });

					boxBounds.setFillColor(sf::Color::Transparent);
					boxBounds.setOutlineColor(sf::Color::Yellow);
					boxBounds.setOutlineThickness(1.f);

					window.draw(boxBounds);
				}
			}

			debugResultText.setString(
				"Player Y: " + std::to_string(player->getPosition().y) +
				"\nPlayer X: " + std::to_string(player->getPosition().x) +
				"\nVelY: " + std::to_string(player->getVelocity().y) +
				"\nCanJump: " + std::string(player->getCanJump() ? "YES" : "NO")
			);

			debugResultText.setPosition(
				camera.getCenter().x - camera.getSize().x / 2.f + 20.f,
				camera.getCenter().y + 200.f
			);

			window.draw(debugResultText);
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