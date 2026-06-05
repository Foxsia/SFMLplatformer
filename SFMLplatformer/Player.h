#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "Entity.h"

namespace fp
{
	enum PLAYER_ANIMATION_STATES { IDLE = 0, MOVING_LEFT, MOVING_RIGHT };

	class Player : public Entity
	{
	public:
		Player();
		~Player();
		
		const sf::Vector2f getPosition() const;
		const sf::FloatRect getGlobalBounds() const;
		const sf::FloatRect getHitbox() const;
		const sf::Vector2f& getVelocity() const;

		inline const bool& getCanJump() const { return canJump; }
		inline void setCanJump(const bool canJump) { this->canJump = canJump; }

		void addScore(int points);
		int getScore() const { return score; };

		void setPosition(const float x, const float y);
		void resetVelocityY();
		void resetVelocityX();

		void hardReset();

		void move(float dir_x, float dir_y, float dt);
		void jump();

		void updatePhysics(float dt);
		void updateMovement();
		void updateAnimation(float dt);
		void update(float dt);

		void render(sf::RenderTarget& target);

	private:
		void initVariables();
		void initTexture();
		void initSprite();
		void initAnimations();
		void initPhysics();

		sf::Sprite sprite;
		sf::Texture textureSheet;

		short animState;
		AnimationComponent* animationComponent = nullptr;

		MovementComponent* movementComponent = nullptr;

		bool canJump;

		int score = 0;
	};
}

