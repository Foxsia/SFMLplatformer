#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
    class TileMap;
    class Player;
    class IState;

    struct GameContext
    {
        sf::RenderWindow* window = nullptr;
        sf::Font* font = nullptr;

        sf::View* camera = nullptr;

        fp::TileMap* tileMap = nullptr;
        fp::Player* player = nullptr;

        // menu state
        size_t* selectedMenuIndex = nullptr;
        std::vector<std::string>* levelFiles = nullptr;

        std::string* currentLevel = nullptr;

        IState** state = nullptr;

        std::map<std::string, sf::Keyboard::Key>* keyboardMappings;
        std::map<std::string, sf::Mouse::Button>* mouseMappings;
    };
}