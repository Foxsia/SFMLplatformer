#include "Player.h"
#include "Portal.h"
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

		const float PLAYER_HITBOX_PADDING_LEFT = 28.f;
		const float PLAYER_HITBOX_PADDING_RIGHT = 28.f;

		const float PLAYER_HITBOX_PADDING_TOP = 28.f;
		const float PLAYER_HITBOX_PADDING_BOTTOM = 0.f;

		const float PORTAL_OFFSET = 10.f;

		const float INVULNERABILITY_DURATION = 10.f;

		const float STAR_VELOCITY_MAX = 750.f;
	}

	void Player::initVariables()
	{
		animState = IDLE;
		invulnerabilityDuration = INVULNERABILITY_DURATION;
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
		animationComponent = std::make_unique<AnimationComponent>(sprite, textureSheet);
	}

	void Player::initPhysics()
	{
		movementComponent = std::make_unique<MovementComponent>(
			sprite,
			VELOCITY_MAX,
			VELOCITY_MIN,
			ACCELERATION,
			DRAG,
			GRAVITY,
			VELOCITY_MAX_Y
		);

		canJump = false;

		damageCooldown = 0.f;
	}

	Player::Player() : Entity(2, 1)
	{
		initVariables();
		initTexture();
		initSprite();
		initAnimations();
		initPhysics();

		health = 1;
	}

	const sf::Vector2f Player::getPosition() const
	{
		return sprite.getPosition();
	}

	const sf::FloatRect Player::getGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}

	const sf::FloatRect Player::getHitbox() const
	{
		auto hitBox = sprite.getGlobalBounds();

		hitBox.left += PLAYER_HITBOX_PADDING_LEFT;
		hitBox.width -= PLAYER_HITBOX_PADDING_LEFT + PLAYER_HITBOX_PADDING_RIGHT;

		hitBox.top += PLAYER_HITBOX_PADDING_TOP;
		hitBox.height -= PLAYER_HITBOX_PADDING_TOP + PLAYER_HITBOX_PADDING_BOTTOM;

		return hitBox;
	}

	const sf::Vector2f& Player::getVelocity() const
	{
		return movementComponent->getVelocity();
	}

	void Player::addScore(int points)
	{
		score += points;
	}

	void Player::setPosition(const float x, const float y)
	{
		sprite.setPosition(x, y);
	}

	void Player::resetVelocityY()
	{
		movementComponent->stopVelocityY();
	}

	void Player::resetVelocityX()
	{
		movementComponent->stopVelocityX();
	}

	void Player::hardReset()
	{
		movementComponent->stopVelocityX();
		movementComponent->stopVelocityY();

		canJump = false;
		damageCooldown = 0.f;

		animState = IDLE;
		heal();
		alive = true;
		score = 0;
		firePower = false;
		invulnerable = false;
		invunerabilityClock.restart();
	}

	void Player::move(float dir_x, float dir_y, float dt)
	{
		movementComponent->move(dir_x, dir_y, dt, speedMultiplier);
	}

	void Player::jump()
	{
		movementComponent->jump(PLAYER_JUMP_FORCE);
		canJump = false;
	}

	void Player::activateInvulnerability()
	{
		invulnerable = true;
		speedMultiplier = 3.5f;

		invunerabilityClock.restart();
	}

	void Player::updatePhysics(float dt)
	{
		movementComponent->update(dt);
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

		if (movementComponent->getVelocity().x > 0.f) facingRight = true;
		if (movementComponent->getVelocity().x < 0.f) facingRight = false;
	}

	void Player::updateAnimation(float dt)
	{
		if (animState == IDLE)
			animationComponent->play("IDLE", dt);
		else
			animationComponent->play("RUN", dt);
	}

	void Player::update(float dt)
	{
		updatePhysics(dt);
		updateMovement();
		updateAnimation(dt);

		if (damageCooldown > 0.f) damageCooldown -= dt;

		if (invulnerable && invunerabilityClock.getElapsedTime().asSeconds() >= invulnerabilityDuration)
		{
			invulnerable = false;
			speedMultiplier = 1.f;

			movementComponent->setVelocityMax(VELOCITY_MAX);
		}

		if (invulnerable)
		{
			movementComponent->setVelocityMax(STAR_VELOCITY_MAX);
			float time = invunerabilityClock.getElapsedTime().asSeconds();

			if (static_cast<int>(time * 10) % 2 == 0)
				sprite.setColor(sf::Color::Yellow);
			else
				sprite.setColor(sf::Color::Magenta);
		}
		else
		{
			sprite.setColor(sf::Color::White);
		}

		if (blockedPortal)
		{
			sf::FloatRect portalBounds = blockedPortal->getGlobalBounds();

			sf::FloatRect expanded(
				portalBounds.left - PORTAL_OFFSET,
				portalBounds.top - PORTAL_OFFSET,
				portalBounds.width + PORTAL_OFFSET,
				portalBounds.height + PORTAL_OFFSET
			);

			if (!getGlobalBounds().intersects(expanded))
			{
				blockedPortal = nullptr;
			}
		}
	}

	void Player::render(sf::RenderTarget& target)
	{
		target.draw(sprite);
	}
}
