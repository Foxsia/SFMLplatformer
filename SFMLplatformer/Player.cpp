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
}

void Player::initPhysics()
{
	velocityMax = 3.f;
	velocityMin = 0.1f;
	acceleration = 0.5f;
	drag = 0.9f;
	gravity = 4.f;
	velocityMaxY = 15.f;
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

void Player::move(const float dir_x, const float dir_y)
{
	//acceleration
	velocity.x += dir_x * acceleration;

	//limit velocity
	if (std::abs(velocity.x) > velocityMax)
	{
		velocity.x = velocityMax * ((velocity.x < 0) ? -1.f : 1.f);
	}
}

void Player::updatePhysics()
{
	//Gravity
	velocity.y += 1.0 * gravity;
	if (std::abs(velocity.x) > velocityMaxY)
	{
		velocity.y = velocityMaxY * ((velocity.y < 0) ? -1.f : 1.f);
	}
	
	//deceleration
	velocity *= drag;

	//limit deceleration
	if (std::abs(velocity.x) < velocityMin) velocity.x = 0.f;
	if (std::abs(velocity.y) < velocityMin) velocity.y = 0.f;

	if (std::abs(velocity.x) <= 0.1f) velocity.x = 0.f;

	sprite.move(velocity);
}

void Player::updateMovement()
{
	animState = IDLE;
	//change it to be outside of player(too many ifs)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))//left
	{
		this->move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))//right
	{
		this->move(1.f, 0.f);
	}

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
	if (animState == IDLE)
	{
		//seperate classes in the future
		if (animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
		{
			currentFrame.top = 0.f;
			currentFrame.left += 32.f;
			if (currentFrame.left >= 96.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}
	}
	else if (animState == MOVING_RIGHT)
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
		{
			currentFrame.top = 64.f;
			currentFrame.left += 32.f;
			if (currentFrame.left >= 224.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}

		sprite.setScale(3.f, 3.f);
		sprite.setOrigin(0.f, 0.f);
	}
	else if (animState == MOVING_LEFT)
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
		{
			currentFrame.top = 64.f;
			currentFrame.left += 32.f;
			if (currentFrame.left >= 224.f) currentFrame.left = 0;

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

void Player::update()
{
	updateMovement();
	updateAnimation();
	updatePhysics();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
