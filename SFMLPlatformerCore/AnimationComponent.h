#pragma once
#include <map>
#include <memory>
#include "Animation.h"

namespace fp
{
	class AnimationComponent
	{
	public:
		AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet);

		void add(const std::string& key, std::unique_ptr<Animation> animation);
		void play(const std::string& key, float dt);

	private:
		sf::Sprite& sprite;
		sf::Texture& textureSheet;

		std::map<std::string, std::unique_ptr<Animation>> animations;
	};
}

