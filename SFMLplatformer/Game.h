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
	sf::Clock dtClock;
	float deltaTime;

	Player* player = nullptr;
	TileMap* tileMap = nullptr;
	std::map<std::string, sf::Keyboard::Key> keyboardMappings;
	std::map<std::string, sf::Mouse::Button> mouseMappings;

	void initWindow();
	void initInput();
	void initTileSheet();
	void initPlayer();
	void initTileMap();
public:
	Game();
	~Game();

	void updateInput();
	void updatePlayer();
	void updateCollision();
	void updateTileCollision();
	void update();

	void renderPlayer();
	void renderTileMap();
	void render();

	const sf::RenderWindow& getWindow() const;
};

