#include "FireBall.h"

namespace fp
{
	namespace
	{
		const float VELOCITY = 500.f;
		const float GRAVITY = 1000.f;

		const float BOUNCE_FORCE = -300.f;
	}
	FireBall::FireBall(sf::Vector2f pos, bool right)
	{
		shape.setRadius(8.f);

		shape.setFillColor(sf::Color::Red);

		shape.setPosition(pos);

		velocity.x = right ? VELOCITY : -VELOCITY;
	}
	void FireBall::update(float dt)
	{
		velocity.y += GRAVITY * dt;
		shape.move(velocity * dt);
	}
	void FireBall::render(sf::RenderTarget& target)
	{
		target.draw(shape);
	}
	sf::FloatRect FireBall::getBounds() const
	{
		return shape.getGlobalBounds();
	}
	void FireBall::setVelocityY(float y)
	{
		velocity.y = y;
	}
	void FireBall::setPosition(float x, float y)
	{
		shape.setPosition(x, y);
	}
	void FireBall::bounce()
	{
		velocity.y = BOUNCE_FORCE;
	}
}
