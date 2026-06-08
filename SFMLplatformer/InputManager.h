#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace fp
{

    struct JoystickAxisBinding
    {
        sf::Joystick::Axis axis;
        float threshold;
    };
    struct ActionBinding
    {
        std::vector<sf::Keyboard::Key> keys;
        std::vector<sf::Mouse::Button> mouseButtons;
        std::vector<unsigned> gamepadButtons;
        std::vector<JoystickAxisBinding> joystickAxes;
    };

	class InputManager
	{
	public:
        void bindKey(const std::string& name, sf::Keyboard::Key key);
        void bindMouse(const std::string& name, sf::Mouse::Button button);
        void bindGamepadButton(const std::string& name, unsigned button);
        void bindJoystickAxis(const std::string& name, sf::Joystick::Axis axis, float threshold);

        bool isActionPressed(const std::string& action) const;

        sf::Vector2f getMouseWorldPosition(const sf::RenderWindow& window) const;

    private:
        std::unordered_map<std::string, ActionBinding> actions;
	};
}

