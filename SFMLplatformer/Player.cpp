#include "Player.h"

void Player::initTexture()
{
}

void Player::initSprite()
{
}

Player::Player()
{
	initTexture();
	initSprite();
}

Player::~Player()
{
}

void Player::update()
{
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
