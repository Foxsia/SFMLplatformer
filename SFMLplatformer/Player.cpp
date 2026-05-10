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
	currentFrame = sf::IntRect(0, 0, 32, 28);
	sprite.setTextureRect(currentFrame);
	sprite.setScale(3.f, 3.f);
}

void Player::initAnimations()
{
	animationTimer.restart();
	animationSwitch = true;
	currentFrame.left = 0.f;
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
}

const bool& Player::getAnimSwitch()
{
	bool anim_switch = animationSwitch;

	if (animationSwitch) animationSwitch = false;

	return anim_switch;
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

void Player::resetAnimationTimer()
{
	animationTimer.restart();
	animationSwitch = true;
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
	}
	else if (velocity.x < 0.f)
	{
		animState = MOVING_LEFT;
	}
	else
	{
		animState = IDLE;
	}
}

void Player::updateAnimation()
{
	float speedPercent = (abs(velocity.x) / velocityMax);

	if (animState == IDLE)
	{
		//seperate classes in the future
		if (animationTimer.getElapsedTime().asMilliseconds() >= 200.f || getAnimSwitch())
		{
			currentFrame.top = 0.f;
			currentFrame.left += 32.f;
			if (currentFrame.left > 96.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}
	}
	else if (animState == MOVING_RIGHT)
	{
		if (animationTimer.getElapsedTime().asMilliseconds() >= 100.f /speedPercent || getAnimSwitch())
		{
			currentFrame.top = 64.f;
			currentFrame.left += 32.f;
			if (currentFrame.left > 224.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}

		sprite.setScale(3.f, 3.f);
		sprite.setOrigin(0.f, 0.f);
	}
	else if (animState == MOVING_LEFT)
	{
		if (animationTimer.getElapsedTime().asMilliseconds() >= 100.f / speedPercent || getAnimSwitch())
		{
			currentFrame.top = 64.f;
			currentFrame.left += 32.f;
			if (currentFrame.left > 224.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}

		sprite.setScale(-3.f, 3.f);
		sprite.setOrigin(sprite.getGlobalBounds().width / 3.f, 0.f);
	}
	else
	{
		animationTimer.restart();
	}
}

void Player::update(const float& delta_time)
{
	updatePhysics(delta_time);
	updateMovement();
	updateAnimation();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
