#pragma once

#include <SFML/Graphics.hpp>

class MovementComponent
{
private:
	sf::Sprite& sprite;

	sf::Vector2f velocity;

	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;

	float gravity;
	float velocityMaxY;

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

	virtual ~MovementComponent();

	const sf::Vector2f& getVelocity() const;

	void move(const float dir_x, const float dir_y, const float& delta_time);
	void jump(const float force);

	void stopVelocityY();

	void update(const float& delta_time);
};