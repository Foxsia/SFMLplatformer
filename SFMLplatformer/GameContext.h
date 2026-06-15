#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include <memory>

namespace fp
{
    class TileMap;
    class Player;
    class IState;
    class Enemy;
    class Collectible;
    class Game;
    class MovingPlatform;

    struct GameContext
    {
        Game* game = nullptr;
        sf::RenderWindow* window = nullptr;
        sf::Font* font = nullptr;

        sf::View* camera = nullptr;

        fp::TileMap* tileMap = nullptr;
        fp::Player* player = nullptr;

        std::vector<std::unique_ptr<Enemy>>* enemies = nullptr;
        std::vector< std::unique_ptr<Collectible>>* collectibles = nullptr;

        size_t* selectedMenuIndex = nullptr;
        std::vector<std::string>* levelFiles = nullptr;

        std::string* currentLevel = nullptr;

        IState* state = nullptr;
        InputManager* input = nullptr;
        bool isEditor = false;

        bool rebuildPlatforms = false;
        std::vector<std::unique_ptr<MovingPlatform>>* platforms;
    };
}