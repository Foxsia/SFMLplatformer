#pragma once
#include <map>
#include "Animation.h"

class AnimationComponent
{
private:
	sf::Sprite& sprite;
	sf::Texture& textureSheet;

	std::map<std::string, Animation*> animations;

public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet);
	~AnimationComponent();

	void add(const std::string& key, Animation* animation);
	void play(const std::string& key, float dt);
};

