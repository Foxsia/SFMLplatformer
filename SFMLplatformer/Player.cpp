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
	currentFrame = sf::IntRect(0, 0, 32, 32);
	sprite.setTextureRect(currentFrame);
	sprite.setScale(5.5f, 5.5f);
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

void Player::updatePhysics()
{

}

void Player::updateMovement()
{
	animState = IDLE;
	//change it to be outside of player(too many ifs)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))//left
	{
		sprite.move(-1.f, 0.f);
		animState = MOVING_LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))//right
	{
		sprite.move(1.f, 0.f);
		animState = MOVING_RIGHT;
	}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))//top
	//{
	//	sprite.move(0.f, -1.f);
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))//down
	//{
	//	sprite.move(0.f, 1.f);
	//}
}

void Player::updateAnimation()
{
	if (animState == IDLE)
	{
		//seperate classes in the future
		if (animationTimer.getElapsedTime().asSeconds() >= 0.2f)
		{
			currentFrame.top = 0.f;
			currentFrame.left += 32.f;
			if (currentFrame.left >= 96.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}
	}
	else if (animState == MOVING_RIGHT)
	{
		if (animationTimer.getElapsedTime().asSeconds() >= 0.1f)
		{
			currentFrame.top = 64.f;
			currentFrame.left += 32.f;
			if (currentFrame.left >= 224.f) currentFrame.left = 0;

			animationTimer.restart();
			sprite.setTextureRect(currentFrame);
		}
	}
	else
	{
		animationTimer.restart();
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
