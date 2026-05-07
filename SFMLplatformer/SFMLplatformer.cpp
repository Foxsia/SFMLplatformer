#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include "Game.h"

int main()
{
    srand(static_cast<unsigned>(time(0)));

    Game game;
    while (game.getWindow().isOpen())
    {
        game.update();
        game.render();
    }
    return 0;
}