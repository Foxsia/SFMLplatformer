#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;
	bool moving;
	sf::Clock animationTimer;

	//animation
	sf::IntRect currentFrame;

	void initVariables();
	void initTexture();
	void initSprite();
	void initAnimations();
public:
	Player();
	virtual ~Player();

	//func
	void updateMovement();
	void updateAnimation();
	void update();
	void render(sf::RenderTarget& target);
};

