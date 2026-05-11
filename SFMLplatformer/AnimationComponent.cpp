#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet)
    : sprite(sprite), textureSheet(textureSheet)
{
    add("IDLE",
        new Animation(sprite,
            0, 0,
            32, 28,
            3,
            40.f));

    add("RUN",
        new Animation(sprite,
            0, 64,
            32, 28,
            8,
            20.f));
}

AnimationComponent::~AnimationComponent()
{
    for (auto& a : animations)
        delete a.second;
}

void AnimationComponent::add(const std::string& key, Animation* animation)
{
    animations[key] = animation;
}

void AnimationComponent::play(const std::string& key, const float& delta_time)
{
    animations[key]->play(delta_time);
}