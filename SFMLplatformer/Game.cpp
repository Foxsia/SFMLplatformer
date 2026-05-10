#include "Game.h"

void Game::initWindow()
{
	window.create(sf::VideoMode(800, 600), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(144);
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
	tileMap = new TileMap(20, 20, &tileSheet, 32);
}

Game::Game()
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
		player->move(-1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(keyboardMappings["KEY_MOVE_RIGHT"]))
	{
		player->move(1.f, 0.f);
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
	player->update();
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

void Game::updateTileMap()
{
	tileMap->update();
}

void Game::update()
{
	//poll window event
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
	}

	updateInput();
	updatePlayer();
	updateCollision();
	updateTileMap();
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
