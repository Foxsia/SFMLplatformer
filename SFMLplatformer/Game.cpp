#include "Game.h"

void Game::initWindow()
{
	window.create(sf::VideoMode(800, 600), "Platformer", sf::Style::Close | sf::Style::Titlebar);
}

void Game::initPlayer()
{
	player = new Player();
}

Game::Game()
{
	initWindow();
	initPlayer();
}

Game::~Game()
{
	delete player;
}

void Game::updatePlayer()
{
	player->update();
}

void Game::update()
{
	//poll window event
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
	}
	updatePlayer();
}

void Game::render()
{
	window.clear();
	window.display();
}

const sf::RenderWindow& Game::getWindow() const
{
	return window;
}
