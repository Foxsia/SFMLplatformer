#pragma once

#include <SFML/Graphics.hpp>

namespace fp
{
    struct GameContext;
    class Player;
    class TileMap;
    class InputManager;

    class CameraController
    {
    public:
        static void followPlayer( sf::View& camera, const Player& player, const TileMap& map);
        static void moveEditor( sf::View& camera, const InputManager& input, const TileMap& map);
    };
}