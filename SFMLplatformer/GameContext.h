#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"

namespace fp
{
    class TileMap;
    class Player;
    class IState;
    class Enemy;

    struct GameContext
    {
        sf::RenderWindow* window = nullptr;
        sf::Font* font = nullptr;

        sf::View* camera = nullptr;

        fp::TileMap* tileMap = nullptr;
        fp::Player* player = nullptr;

        std::vector<Enemy*>* enemies = nullptr;

        size_t* selectedMenuIndex = nullptr;
        std::vector<std::string>* levelFiles = nullptr;

        std::string* currentLevel = nullptr;

        IState** state = nullptr;
        InputManager* input = nullptr;
        bool isEditor = false;
    };
}