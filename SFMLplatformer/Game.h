#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include "TileMap.h"

class Game
{
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Texture tileSheet;

	Player* player;
	TileMap* tileMap;

	void initWindow();
	void initTileSheet();
	void initPlayer();
	void initTileMap();
public:
	Game();
	virtual ~Game();

	//functions
	void updatePlayer();
	void updateCollision();
	void updateTileMap();
	void update();

	void renderPlayer();
	void renderTileMap();
	void render();

	const sf::RenderWindow& getWindow() const;
};

