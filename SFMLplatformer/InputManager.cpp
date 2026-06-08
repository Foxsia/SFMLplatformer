#include "InputManager.h"

namespace fp
{
    void InputManager::bindKey(const std::string& name, sf::Keyboard::Key key)
    {
        actions[name].keys.push_back(key);
    }

    void InputManager::bindMouse(const std::string& name, sf::Mouse::Button button)
    {
        actions[name].mouseButtons.push_back(button);
    }

    void InputManager::bindGamepadButton(const std::string& name, unsigned button)
    {
        actions[name].gamepadButtons.push_back(button);
    }

    void InputManager::bindJoystickAxis(const std::string& name, sf::Joystick::Axis axis, float threshold)
    {
        actions[name].joystickAxes.push_back({axis, threshold});
    }

    bool InputManager::isActionPressed(const std::string& action) const
    {
        auto it = actions.find(action);

        if (it == actions.end())
            return false;

        const auto& binding = it->second;

        for (auto key : binding.keys)
        {
            if (sf::Keyboard::isKeyPressed(key))
                return true;
        }

        for (auto button : binding.mouseButtons)
        {
            if (sf::Mouse::isButtonPressed(button))
                return true;
        }

        if (sf::Joystick::isConnected(0))
        {
            for (auto button : binding.gamepadButtons)
            {
                if (sf::Joystick::isButtonPressed(0, button))
                    return true;
            }

            for (const auto& axisBinding : binding.joystickAxes)
            {
                float value = sf::Joystick::getAxisPosition(
                    0,
                    axisBinding.axis
                );

                if (axisBinding.threshold > 0.f)
                {
                    if (value > axisBinding.threshold)
                        return true;
                }
                else
                {
                    if (value < axisBinding.threshold)
                        return true;
                }
            }
        }

        return false;
    }
    sf::Vector2f InputManager::getMouseWorldPosition(const sf::RenderWindow& window) const
    {
       const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        return window.mapPixelToCoords(pixelPos);
    }
}