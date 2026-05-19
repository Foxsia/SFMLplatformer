#include "Animation.h"

namespace fp
{
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

	void Animation::play(float delta_time)
	{
		const float ANIMATION_SPEED = 200.f;

		timer += ANIMATION_SPEED * delta_time;

		if (timer >= delay)
		{
			timer = 0.f;

			currentRect.left += width;

			const int animationEnd = startX + (width * frameCount);

			if (currentRect.left >= animationEnd)
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
}
