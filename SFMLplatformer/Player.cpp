#include "Player.h"
#include <iostream>

void Player::initVariables()
{
	moving = false;
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
	currentFrame = sf::IntRect(0, 0, 32, 32);
	sprite.setTextureRect(currentFrame);
	sprite.setScale(2.5f, 2.5f);
}

void Player::initAnimations()
{
	animationTimer.restart();
}

Player::Player()
{
	initVariables();
	initTexture();
	initSprite();
	initAnimations();
}

Player::~Player()
{
}

void Player::updateMovement()
{
	moving = false;
	//change it to be outside of player(too many ifs)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))//left
	{
		sprite.move(-1.f, 0.f);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))//right
	{
		sprite.move(1.f, 0.f);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))//top
	{
		sprite.move(0.f, -1.f);
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))//down
	{
		sprite.move(0.f, 1.f);
		moving = true;
	}
}

void Player::updateAnimation()
{
	//seperate classes in the future
	if (animationTimer.getElapsedTime().asSeconds() >= 0.2f)
	{
		if (!moving)
		{
			currentFrame.left += 32.f;
			if (currentFrame.left >= 96.f) currentFrame.left = 0;
		}

		animationTimer.restart();
		sprite.setTextureRect(currentFrame);
	}
}

void Player::update()
{
	updateMovement();
	updateAnimation();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
