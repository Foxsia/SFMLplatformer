#include "FireBall.h"

namespace fp
{
	namespace
	{
		const float VELOCITY = 500.f;
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
}
