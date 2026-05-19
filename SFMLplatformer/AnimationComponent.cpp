#include "AnimationComponent.h"

namespace fp
{
    namespace
    {
        const int FRAME_WIDTH = 32;
        const int FRAME_HEIGHT = 28;

        const int IDLE_START_X = 0;
        const int IDLE_START_Y = 0;
        const int IDLE_FRAME_COUNT = 3;
        const float IDLE_DELAY = 40.f;

        const int RUN_START_X = 0;
        const int RUN_START_Y = 64;
        const int RUN_FRAME_COUNT = 8;
        const float RUN_DELAY = 20.f;
    }

    AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet)
        : sprite(sprite), textureSheet(textureSheet)
    {
        add("IDLE",
            new Animation(sprite,
                IDLE_START_X, IDLE_START_Y,
                FRAME_WIDTH, FRAME_HEIGHT,
                IDLE_FRAME_COUNT,
                IDLE_DELAY));

        add("RUN",
            new Animation(sprite,
                RUN_START_X, RUN_START_Y,
                FRAME_WIDTH, FRAME_HEIGHT,
                RUN_FRAME_COUNT,
                RUN_DELAY));
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

    void AnimationComponent::play(const std::string& key, float delta_time)
    {
        animations[key]->play(delta_time);
    }
}