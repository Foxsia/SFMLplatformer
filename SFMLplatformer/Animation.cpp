#include "Animation.h"

Animation::Animation(sf::Sprite& sprite, int startX, int startY, int width, int height, int frameCount, float delay) 
	: sprite(sprite),
	startX(startX), startY(startY),
	width(width), height(height),
	frameCount(frameCount),
	delay(delay)
{
	timer = 0.f;
	currentRect = sf::IntRect(startX, startY, width, height);
	sprite.setTextureRect(currentRect);
}

void Animation::play(const float& delta_time)
{
	timer += 200.f * delta_time;

	if (timer >= delay)
	{
		timer = 0.f;

		currentRect.left += width;

		if (currentRect.left >= startX + width * frameCount)
		{
			currentRect.left = startX;
		}

		sprite.setTextureRect(currentRect);
	}
}

void Animation::reset()
{
	timer = 0.f;
	currentRect.left = startX;
	sprite.setTextureRect(currentRect);
}
