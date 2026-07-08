#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	struct GameContext;

	class IState
	{
	public:
		virtual ~IState() = default;

		virtual void update(float dt, GameContext& context) = 0;
		virtual void render(sf::RenderWindow& window, GameContext& context) = 0;
		virtual void handleEvent(const sf::Event& event) {}
		virtual bool blocksGameInput() const { return false; }
	};
}