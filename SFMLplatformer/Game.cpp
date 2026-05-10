#include "Game.h"

void Game::initWindow()
{
	window.create(sf::VideoMode(800, 600), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(144);
}

void Game::initTileSheet()
{
	if (!tileSheet.loadFromFile("assets/player.png"))
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
	tileMap->addTile(0, 0);
}

Game::Game()
{
	initWindow();
	initTileSheet();
	initPlayer();
	initTileMap();
}

Game::~Game()
{
	delete player;
	delete tileMap;
}

void Game::updatePlayer()
{
	player->update();
}

void Game::updateCollision()
{
	if (player->getPosition().y + player->getGlobalBounds().height > window.getSize().y)
	{
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

		if (event.type == sf::Event::KeyReleased &&
			(event.key.code == sf::Keyboard::A ||
			event.key.code == sf::Keyboard::D ||
			event.key.code == sf::Keyboard::W ||
			event.key.code == sf::Keyboard::S)) player->resetAnimationTimer();
	}

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
