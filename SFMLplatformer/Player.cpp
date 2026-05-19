#include "Player.h"
#include <iostream>

namespace fp
{
	namespace
	{
		const float PLAYER_SCALE_X = 3.f;
		const float PLAYER_SCALE_Y = 3.f;

		const float PLAYER_FLIPPED_SCALE_X = -3.f;

		const float SPRITE_ORIGIN_X = 0.f;
		const float SPRITE_ORIGIN_Y = 0.f;

		const float PLAYER_JUMP_FORCE = 700.f;

		const float VELOCITY_MAX = 500.f;
		const float VELOCITY_MIN = 1.f;

		const float ACCELERATION = 4000.f;

		const float DRAG = 0.95f;

		const float GRAVITY = 1500.f;

		const float VELOCITY_MAX_Y = 1000.f;

		const float RIGHT_MOVEMENT_THRESHOLD = 0.f;
		const float LEFT_MOVEMENT_THRESHOLD = 0.f;
	}

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
		sprite.setScale(PLAYER_SCALE_X, PLAYER_SCALE_Y);
	}

	void Player::initAnimations()
	{
		animationComponent = new AnimationComponent(sprite, textureSheet);
	}

	void Player::initPhysics()
	{
		movementComponent = new MovementComponent(
			sprite,
			VELOCITY_MAX,
			VELOCITY_MIN,
			ACCELERATION,
			DRAG,
			GRAVITY,
			VELOCITY_MAX_Y
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
		movementComponent->jump(PLAYER_JUMP_FORCE);
		canJump = false;
	}

	void Player::updatePhysics(float delta_time)
	{
		movementComponent->update(delta_time);
	}

	void Player::updateMovement()
	{
		if (movementComponent->getVelocity().x > RIGHT_MOVEMENT_THRESHOLD)
		{
			animState = MOVING_RIGHT;
			sprite.setScale(PLAYER_SCALE_X, PLAYER_SCALE_Y);
			sprite.setOrigin(SPRITE_ORIGIN_X, SPRITE_ORIGIN_Y);
		}
		else if (movementComponent->getVelocity().x < 0.f)
		{
			animState = MOVING_LEFT;
			sprite.setScale(PLAYER_FLIPPED_SCALE_X, PLAYER_SCALE_Y);
			sprite.setOrigin(sprite.getGlobalBounds().width / PLAYER_SCALE_X, SPRITE_ORIGIN_Y);
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
}
