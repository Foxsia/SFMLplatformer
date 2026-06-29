#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	enum class Team
	{
		Player,
		Enemy
	};

	class FireBall
	{
	public:
		FireBall(sf::Vector2f pos, bool right, Team team);

		void update(float dt);

		void render(sf::RenderTarget& target);

		sf::FloatRect getBounds() const;

		sf::Vector2f getVelocity() const { return velocity; }

		void setVelocityY(float y);
		void setPosition(float x, float y);
		sf::Vector2f getPosition() { return shape.getPosition(); }

		void destroy() { alive = false; }

		void bounce();

		bool isAlive() const { return alive; }

		Team getTeam() const { return team; }

	private:
		sf::CircleShape shape;

		sf::Vector2f velocity;

		bool alive = true;
		Team team;
	};
}

