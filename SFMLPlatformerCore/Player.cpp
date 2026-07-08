#include "Player.h"
#include "Portal.h"
#include <iostream>
#include "ConfigManager.h"

namespace fp
{
	namespace
	{
		const float PLAYER_SCALE_X = 3.f;
		const float PLAYER_SCALE_Y = 3.f;

		const float PLAYER_FLIPPED_SCALE_X = -3.f;

		const float SPRITE_ORIGIN_X = 0.f;
		const float SPRITE_ORIGIN_Y = 0.f;

		const float RIGHT_MOVEMENT_THRESHOLD = 0.f;
		const float LEFT_MOVEMENT_THRESHOLD = 0.f;
	}

	void Player::initVariables()
	{
		animState = IDLE;
		invulnerabilityDuration = cfg->at("invulnerabilityDuration").get<float>();
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
			cfg->at("velocityMax").get<float>(),
			cfg->at("velocityMin").get<float>(),
			cfg->at("acceleration").get<float>(),
			cfg->at("drag").get<float>(),
			cfg->at("gravity").get<float>(),
			cfg->at("velocityMaxY").get<float>()
		);

		canJump = false;

		damageCooldown = 0.f;
	}

	void Player::setSpriteColor(const sf::Color& color)
	{
		sprite.setColor(color);
	}

	Player::Player() : Entity(2, 1)
	{
		cfg = &ConfigManager::get("player");
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

		float left = cfg->at("hitboxPaddingLeft").get<float>();
		float right = cfg->at("hitboxPaddingRight").get<float>();
		float top = cfg->at("hitboxPaddingTop").get<float>();
		float bottom = cfg->at("hitboxPaddingBottom").get<float>();

		hitBox.left += left;
		hitBox.width -= left + right;

		hitBox.top += top;
		hitBox.height -= top + bottom;

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
		speedMultiplier = 1.f;
	}

	void Player::move(float dir_x, float dir_y, float dt)
	{
		movementComponent->move(dir_x, dir_y, dt, speedMultiplier);
	}

	void Player::jump()
	{
		movementComponent->jump(cfg->at("jumpForce").get<float>());
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

		updateDamageFlash();

		if (damageCooldown > 0.f) damageCooldown -= dt;

		if (invulnerable && invunerabilityClock.getElapsedTime().asSeconds() >= invulnerabilityDuration)
		{
			invulnerable = false;
			speedMultiplier = 1.f;

			movementComponent->setVelocityMax(cfg->at("velocityMax").get<float>());
		}
		if (!damageFlash)
		{
			if (invulnerable)
			{
				movementComponent->setVelocityMax(cfg->at("starVelocityMax").get<float>());
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
		}

		if (blockedPortal)
		{
			sf::FloatRect portalBounds = blockedPortal->getGlobalBounds();

			float offset = cfg->at("portalOffset").get<float>();

			sf::FloatRect expanded(
				portalBounds.left - offset,
				portalBounds.top - offset,
				portalBounds.width + offset,
				portalBounds.height + offset
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
