#include "Player.h"
#include <iostream>

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
}

Player::Player()
{
	initTexture();
	initSprite();
}

Player::~Player()
{
}

void Player::updateMovement()
{
	//change it to be outside of player(too many ifs)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))//left
	{
		sprite.move(-1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))//right
	{
		sprite.move(1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))//top
	{
		sprite.move(0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))//down
	{
		sprite.move(0.f, 1.f);
	}
}

void Player::update()
{
	updateMovement();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
