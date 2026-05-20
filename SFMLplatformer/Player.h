#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "AnimationComponent.h"
#include "MovementComponent.h"

namespace fp
{
	enum PLAYER_ANIMATION_STATES { IDLE = 0, MOVING_LEFT, MOVING_RIGHT };

	class Player
	{
	public:
		Player();
		~Player();

		//geters
		const sf::Vector2f getPosition() const;
		const sf::FloatRect getGlobalBounds() const;
		const sf::Vector2f& getVelocity() const;

		//inline func
		inline const bool& getCanJump() const { return canJump; }
		inline void setCanJump(const bool canJump) { this->canJump = canJump; }

		//modifiers
		void setPosition(const float x, const float y);
		void resetVelocityY();

		//func
		void move(float dir_x, float dir_y, float delta_time);
		void jump();

		void updatePhysics(float delta_time);
		void updateMovement();
		void updateAnimation(float delta_time);
		void update(float delta_time);

		void render(sf::RenderTarget& target);

	private:
		void initVariables();
		void initTexture();
		void initSprite();
		void initAnimations();
		void initPhysics();

		sf::Sprite sprite;
		sf::Texture textureSheet;

		//animation
		short animState;
		AnimationComponent* animationComponent = nullptr;

		//physics
		MovementComponent* movementComponent = nullptr;

		bool canJump;
	};
}

