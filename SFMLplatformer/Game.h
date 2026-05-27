#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <vector>
#include "Player.h"
#include "TileMap.h"
#include "IState.h"
#include "InputManager.h"
#include "Enemy.h"

namespace fp
{
	class Game
	{
	public:
		Game();
		~Game();

		void updatePlayer();
		void update();
		
		void renderPlayer();
		void renderTileMap();
		void render();

		void loadLevelList();

		const sf::RenderWindow& getWindow() const;

	private:
		void initWindow();
		void initInput();
		void initTileSheet();
		void initPlayer();
		void initTileMap();

		class IState;
		fp::IState* state = nullptr;
		std::string currentLevel;

		sf::RenderWindow window;
		sf::View camera;
		sf::Event event;
		sf::Texture tileSheet;
		sf::Clock dtClock;
		float deltaTime;

		Player* player = nullptr;
		TileMap* tileMap = nullptr;

		std::vector<Enemy*> enemies;

		InputManager* input;
		//file saving
		bool typingFileName = false;
		std::string fileNameInput;

		sf::Font font;
		sf::Text fileNameText;
		//editor help
		sf::Text editorHelp;
		//menu
		std::vector<std::string> levelFiles;
		std::size_t selectedMenuIndex = 0;
		sf::Text menuText;
		sf::Text titleMenuText;
	};
}

