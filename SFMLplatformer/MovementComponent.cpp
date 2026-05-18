#include "MovementComponent.h"
#include <cmath>

MovementComponent::MovementComponent(
	sf::Sprite& sprite,
	float velocityMax,
	float velocityMin,
	float acceleration,
	float drag,
	float gravity,
	float velocityMaxY
)
	: sprite(sprite), 
	velocityMax(velocityMax),
	velocityMin(velocityMin),
	acceleration(acceleration),
	drag(drag),
	gravity(gravity),
	velocityMaxY(velocityMaxY)
{}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return velocity;
}

void MovementComponent::move(
	float dir_x,
	float dir_y,
	float delta_time
)
{
	velocity.x += dir_x * acceleration * delta_time;
	velocity.y += dir_y * acceleration * delta_time;

	// limit velocity X
	if (std::abs(velocity.x) > velocityMax)
	{
		velocity.x = velocityMax * (
			(velocity.x < 0.f) ? -1.f : 1.f
			);
	}
}

void MovementComponent::jump(float force)
{
	velocity.y = -force;
}

void MovementComponent::stopVelocityY()
{
	velocity.y = 0.f;
}

void MovementComponent::update(float delta_time)
{
	// gravity
	velocity.y += gravity * delta_time;

	if (std::abs(velocity.y) > velocityMaxY)
	{
		velocity.y = velocityMaxY;
	}

	// drag
	velocity.x *= drag;

	// min velocity
	if (std::abs(velocity.x) < velocityMin)
	{
		velocity.x = 0.f;
	}

	if (std::abs(velocity.y) < velocityMin)
	{
		velocity.y = 0.f;
	}

	if (std::abs(velocity.x) <= 0.1f)
	{
		velocity.x = 0.f;
	}

	sprite.move(velocity * delta_time);
}