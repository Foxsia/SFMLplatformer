#pragma once

#include <SFML/Graphics.hpp>

class MovementComponent
{
public:
	MovementComponent(
		sf::Sprite& sprite,
		float velocityMax,
		float velocityMin,
		float acceleration,
		float drag,
		float gravity,
		float velocityMaxY
	);

	const sf::Vector2f& getVelocity() const;

	void move(float dir_x, float dir_y, float delta_time);
	void jump(float force);

	void stopVelocityY();

	void update(float delta_time);

private:
	sf::Sprite& sprite;

	sf::Vector2f velocity;

	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;

	float gravity;
	float velocityMaxY;
};