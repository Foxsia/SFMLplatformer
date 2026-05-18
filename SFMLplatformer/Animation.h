#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
private:
	sf::Sprite& sprite;

	sf::IntRect currentRect;

	float timer;
	float delay;

	int frameCount;
	int startX;
	int startY;
	int width;
	int height;

public:
	Animation(sf::Sprite& sprite, int startX, int startY, int width, int height, int frameCount, float delay);

	void play(float delta_time);
	void reset();
};

