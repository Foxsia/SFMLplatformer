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
	: sprite(sprite)
{
	this->velocityMax = velocityMax;
	this->velocityMin = velocityMin;
	this->acceleration = acceleration;
	this->drag = drag;
	this->gravity = gravity;
	this->velocityMaxY = velocityMaxY;
}

MovementComponent::~MovementComponent()
{
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return velocity;
}

void MovementComponent::move(
	const float dir_x,
	const float dir_y,
	const float& delta_time
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

void MovementComponent::jump(const float force)
{
	velocity.y = -force;
}

void MovementComponent::stopVelocityY()
{
	velocity.y = 0.f;
}

void MovementComponent::update(const float& delta_time)
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