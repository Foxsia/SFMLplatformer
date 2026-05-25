#include "InputManager.h"

namespace fp
{
    void InputManager::bindKey(const std::string& name, sf::Keyboard::Key key)
    {
        keys[name] = key;
    }

    void InputManager::bindMouse(const std::string& name, sf::Mouse::Button button)
    {
        mouseButtons[name] = button;
    }

    bool InputManager::isKeyDown(const std::string& name) const
    {
        auto it = keys.find(name);
        if (it == keys.end()) return false;

        return sf::Keyboard::isKeyPressed(it->second);
    }

    bool InputManager::isMouseDown(const std::string& name) const
    {
        auto it = mouseButtons.find(name);
        if (it == mouseButtons.end()) return false;

        return sf::Mouse::isButtonPressed(it->second);
    }

    sf::Vector2f InputManager::getMouseWorldPosition(const sf::RenderWindow& window) const
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        return window.mapPixelToCoords(pixelPos);
    }
}