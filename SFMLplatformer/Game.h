#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"

class Game
{
private:
	sf::RenderWindow window;
	sf::Event event;

	Player* player;

	void initWindow();
	void initPlayer();
public:
	Game();
	virtual ~Game();

	//functions
	void updatePlayer();
	void update();
	void render();
	const sf::RenderWindow& getWindow() const;
};

