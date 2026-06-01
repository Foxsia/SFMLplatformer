#include "Game.h"
#include "MenuState.h"
#include "EditorState.h"
#include "GameContext.h"
#include "LevelLoader.h"
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

		font.loadFromFile("assets/fonts/Roboto_Condensed-Black.ttf");

		fileNameText.setFont(font);
		fileNameText.setCharacterSize(24);
		fileNameText.setFillColor(sf::Color::White);
	}

	void Game::initInput()
	{
		input = new fp::InputManager();

		input->bindKey("MOVE_LEFT", sf::Keyboard::A);
		input->bindKey("MOVE_RIGHT", sf::Keyboard::D);
		input->bindKey("JUMP", sf::Keyboard::Space);
		input->bindKey("TILE_BRUSH", sf::Keyboard::Num1);
		input->bindKey("ENEMY_BRUSH", sf::Keyboard::Num2);

		input->bindMouse("ADD_ELEMENT", sf::Mouse::Left);
		input->bindMouse("REMOVE_ELEMENT", sf::Mouse::Right);
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
		state = new fp::MenuState();
		loadLevelList();
	}

	Game::~Game()
	{
		delete player;
		delete tileMap;
		delete state;

		for (auto enemy : enemies)
		{
			delete enemy;
		}
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

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::E)
				{
					delete state;
					state = new fp::EditorState();
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
					tileMap->saveToFile("levels/" + fileNameInput + ".txt", enemies);

					loadLevelList();

					typingFileName = false;
				}
				if (typingFileName && event.key.code == sf::Keyboard::BackSpace)
				{
					if (!fileNameInput.empty()) fileNameInput.pop_back();
				}

				if (event.key.code == sf::Keyboard::M)
				{
					tileMap->clear();

					for (auto enemy : enemies)
					{
						delete enemy;
					}
					enemies.clear();

					delete state;
					state = new fp::MenuState();
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

		fp::GameContext context;

		context.window = &window;
		context.font = &font;

		context.tileMap = tileMap;
		context.player = player;
		context.enemies = &enemies;

		context.selectedMenuIndex = &selectedMenuIndex;
		context.levelFiles = &levelFiles;

		context.currentLevel = &currentLevel;

		context.state = &state;
		context.isEditor = false;
		context.camera = &camera;

		context.input = input;
		
		if (state)
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

		context.tileMap = tileMap;
		context.player = player;
		context.enemies = &enemies;

		context.selectedMenuIndex = &selectedMenuIndex;
		context.levelFiles = &levelFiles;

		context.currentLevel = &currentLevel;

		context.state = &state;
		context.isEditor = false;
		context.camera = &camera;

		context.input = input;

		if (state)
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