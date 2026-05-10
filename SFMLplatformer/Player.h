#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "AnimationComponent.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;

	//animation
	short animState;
	AnimationComponent* animationComponent;

	//physics
	sf::Vector2f velocity;
	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;
	float gravity;
	float velocityMaxY;

	bool canJump;

	void initVariables();
	void initTexture();
	void initSprite();
	void initAnimations();
	void initPhysics();
public:
	Player();
	virtual ~Player();

	//geters
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//inline func
	inline const bool& getCanJump() const { return canJump; }
	inline void setCanJump(const bool canJump) { this->canJump = canJump; }

	//modifiers
	void setPosition(const float x, const float y);
	void resetVelocityY();

	//func
	void move(const float dir_x, const float dir_y, const float& delta_time);
	void jump();
	void updatePhysics(const float& delta_time);
	void updateMovement();
	void updateAnimation(const float& delta_time);
	void update(const float& delta_time);
	void render(sf::RenderTarget& target);
};

