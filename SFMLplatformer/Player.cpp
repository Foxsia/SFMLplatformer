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
	movementComponent = new MovementComponent(
		sprite,
		500.f,
		1.f,
		4000.f,
		0.95f,
		1500.f,
		1000.f
	);

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
	delete movementComponent;
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
	movementComponent->stopVelocityY();
}

void Player::move(float dir_x, float dir_y, float delta_time)
{
	movementComponent->move(dir_x, dir_y, delta_time);
}

void Player::jump()
{
	movementComponent->jump(700.f);
	canJump = false;
}

void Player::updatePhysics(float delta_time)
{
	movementComponent->update(delta_time);
}

void Player::updateMovement()
{
	if (movementComponent->getVelocity().x > 0.f)
	{
		animState = MOVING_RIGHT;
		sprite.setScale(3.f, 3.f);
		sprite.setOrigin(0.f, 0.f);
	}
	else if (movementComponent->getVelocity().x < 0.f)
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

void Player::updateAnimation(float delta_time)
{
	if (animState == IDLE)
		animationComponent->play("IDLE", delta_time);
	else
		animationComponent->play("RUN", delta_time);
}

void Player::update(float delta_time)
{
	updatePhysics(delta_time);
	updateMovement();
	updateAnimation(delta_time);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
