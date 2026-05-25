#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace fp
{
	class InputManager
	{
	public:
        void bindKey(const std::string& name, sf::Keyboard::Key key);
        void bindMouse(const std::string& name, sf::Mouse::Button button);

        bool isKeyDown(const std::string& name) const;
        bool isMouseDown(const std::string& name) const;

        sf::Vector2f getMouseWorldPosition(const sf::RenderWindow& window) const;

    private:
        std::unordered_map<std::string, sf::Keyboard::Key> keys;
        std::unordered_map<std::string, sf::Mouse::Button> mouseButtons;
	};
}

