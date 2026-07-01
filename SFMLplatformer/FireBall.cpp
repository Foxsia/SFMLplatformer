#include "FireBall.h"
#include "ConfigManager.h"

namespace fp
{
	FireBall::FireBall(sf::Vector2f pos, bool right, Team team) : team(team)
	{
		const auto& cfg = ConfigManager::get("fireball");
		shape.setRadius(8.f);

		shape.setFillColor(sf::Color::Red);

		shape.setPosition(pos);

		velocity.x = right ? cfg["velocity"].get<float>() : -cfg["velocity"].get<float>();
	}
	void FireBall::update(float dt)
	{ 
		if(this->getTeam() == Team::Player) velocity.y += ConfigManager::get("fireball")["gravity"].get<float>() * dt;
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
		velocity.y = ConfigManager::get("fireball")["bounceForce"].get<float>();
	}
}
