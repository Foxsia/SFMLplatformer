#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
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
	std::map<std::string, sf::Keyboard::Key> keyboardMappings;
	std::map<std::string, sf::Mouse::Button> mouseMappings;

	void initWindow();
	void initInput();
	void initTileSheet();
	void initPlayer();
	void initTileMap();
public:
	Game();
	virtual ~Game();

	//functions
	void updateInput();
	void updatePlayer();
	void updateCollision();
	void updateTileMap();
	void update();

	void renderPlayer();
	void renderTileMap();
	void render();

	const sf::RenderWindow& getWindow() const;
};

