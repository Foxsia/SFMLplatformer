#include "Player.h"
#include <iostream>

void Player::initVariables()
{
	animState = IDLE;
}

void Player::initTexture()
{
	if (!textureSheet.loadFromFile("assets/player.png"))
	{
		std::cout << "Error::Player::Couldn't load the player sheet!\n";
	}
}

void Player::initSprite()
{
	sprite.setTexture(textureSheet);
	sprite.setScale(3.f, 3.f);
}

void Player::initAnimations()
{
	animationComponent = new AnimationComponent(sprite, textureSheet);
}

void Player::initPhysics()
{
	velocityMax = 500.f;
	velocityMin = 1.f;
	acceleration = 4000.f;
	drag = 0.95f;
	gravity = 1500.f;
	velocityMaxY = 1000.f;
	canJump = false;
}

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	initAnimations();
	initPhysics();
}

Player::~Player()
{
	delete animationComponent;
}

const sf::Vector2f Player::getPosition() const
{
	return sprite.getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	sprite.setPosition(x, y);
}

void Player::resetVelocityY()
{
	velocity.y = 0.f;
}

void Player::move(const float dir_x, const float dir_y, const float& delta_time)
{
	//acceleration
	velocity.x += dir_x * acceleration * delta_time;

	//limit velocity
	if (std::abs(velocity.x) > velocityMax)
	{
		velocity.x = velocityMax * ((velocity.x < 0.f) ? -1.f : 1.f);
	}
}

void Player::jump()
{
	velocity.y = -700.f;
	canJump = false;
}

void Player::updatePhysics(const float& delta_time)
{
	//Gravity
	velocity.y += delta_time * gravity;
	if (std::abs(velocity.y) > velocityMaxY)
	{
		velocity.y = velocityMaxY;
	}
	//deceleration
	velocity.x *= drag;

	//limit deceleration
	if (std::abs(velocity.x) < velocityMin) velocity.x = 0.f;
	if (std::abs(velocity.y) < velocityMin) velocity.y = 0.f;

	if (std::abs(velocity.x) <= 0.1f) velocity.x = 0.f;

	sprite.move(velocity * delta_time);
}

void Player::updateMovement()
{
	if (velocity.x > 0.f)
	{
		animState = MOVING_RIGHT;
		sprite.setScale(3.f, 3.f);
		sprite.setOrigin(0.f, 0.f);
	}
	else if (velocity.x < 0.f)
	{
		animState = MOVING_LEFT;
		sprite.setScale(-3.f, 3.f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 3.f, 0.f);
	}
	else
	{
		animState = IDLE;
	}
}

void Player::updateAnimation(const float& delta_time)
{
	if (animState == IDLE)
		animationComponent->play("IDLE", delta_time);
	else
		animationComponent->play("RUN", delta_time);
}

void Player::update(const float& delta_time)
{
	updatePhysics(delta_time);
	updateMovement();
	updateAnimation(delta_time);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
