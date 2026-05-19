#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include "Game.h"

int main()
{
    std::srand(static_cast<unsigned>(time(0)));

    fp::Game game;
    while (game.getWindow().isOpen())
    {
        game.update();
        game.render();
    }
    return 0;
}