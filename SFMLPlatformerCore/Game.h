#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <vector>
#include <memory>
#include "Player.h"
#include "TileMap.h"
#include "IState.h"
#include "InputManager.h"
#include "Enemy.h"
#include "LifeFruit.h"
#include "FireFruit.h"
#include "InvulnerabilityFruit.h"
#include "FireBall.h"
#include "Goal.h"
#include "MovingPlatform.h"
#include "StateManager.h"

namespace fp
{
	class Game
	{
	public:
		Game();

		void updatePlayer();
		void update();
		
		void renderPlayer();
		void renderTileMap();
		void render();

		void loadLevelList();
		void buildMovingPlatforms();
		void restartLevelTimer();

		void setTypingFileName(bool isTyping);
		void setShowDebug(bool show);
		const bool getTypingFileName() { return typingFileName; }
		std::string& getFileNameInput() { return fileNameInput; }
		void saveLevel();
		const sf::RenderWindow& getWindow() const;

		float getLevelRunTime() const { return levelTimer.getElapsedTime().asSeconds(); }

	private:
		void initWindow();
		void initInput();
		void initTileSheet();
		void initPlayer();
		void initTileMap();
		void initCreators();

		StateManager stateManager;
		std::string currentLevel;

		sf::RenderWindow window;
		sf::View camera;
		sf::Event event;
		sf::Texture tileSheet;
		sf::Clock dtClock;
		float deltaTime;

		std::unique_ptr<Player> player;
		std::unique_ptr<TileMap> tileMap;

		std::vector<std::unique_ptr<Enemy>> enemies;
		std::vector<std::unique_ptr<Collectible>> collectibles;
		std::vector<std::unique_ptr<FireBall>> fireballs;
		std::vector<std::unique_ptr<MovingPlatform>> movingPlatforms;

		std::unique_ptr<InputManager> input;

		bool typingFileName = false;
		std::string fileNameInput;

		sf::Font font;
		sf::Text fileNameText;

		sf::Text editorHelp;

		std::vector<std::string> levelFiles;
		std::size_t selectedMenuIndex = 0;
		sf::Text menuText;
		sf::Text titleMenuText;

		sf::Clock levelTimer;
		float levelDuration;

		bool showDebugMenu = false;
		sf::Text debugText;
		sf::Text debugResultText;
		sf::Text teleportResultText;
		bool showTeleportError = false;
		sf::Clock teleportErrorClock;

		bool typingTeleport = false;
		std::string teleportInput;

		float teleportX = 0.f;
		float teleportY = 0.f;
	};
}

