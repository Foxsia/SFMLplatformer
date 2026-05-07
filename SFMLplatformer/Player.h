#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;
	sf::Clock animationTimer;

	//animation
	short animState;
	sf::IntRect currentFrame;

	//physics
	sf::Vector2f velocity;
	float acceleration;
	float deceleration;

	void initVariables();
	void initTexture();
	void initSprite();
	void initAnimations();
public:
	Player();
	virtual ~Player();

	//func
	void updatePhysics();
	void updateMovement();
	void updateAnimation();
	void update();
	void render(sf::RenderTarget& target);
};

