#include "AnimationComponent.h"
#include "ConfigManager.h"

namespace fp
{
    namespace
    {
        const int FRAME_WIDTH = 32;
        const int FRAME_HEIGHT = 28;

        const int IDLE_START_X = 0;
        const int IDLE_START_Y = 0;
        const int IDLE_FRAME_COUNT = 3;

        const int RUN_START_X = 0;
        const int RUN_START_Y = 64;
        const int RUN_FRAME_COUNT = 8;
    }

    AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet)
        : sprite(sprite), textureSheet(textureSheet)
    {
        const auto& cfg = ConfigManager::get("animations");
        add("IDLE",
            std::make_unique<Animation>(sprite,
                IDLE_START_X, IDLE_START_Y,
                FRAME_WIDTH, FRAME_HEIGHT,
                IDLE_FRAME_COUNT,
                cfg["idleDelay"]));

        add("RUN",
            std::make_unique<Animation>(sprite,
                RUN_START_X, RUN_START_Y,
                FRAME_WIDTH, FRAME_HEIGHT,
                RUN_FRAME_COUNT,
                cfg["runDelay"]));
    }

    void AnimationComponent::add(const std::string& key, std::unique_ptr<Animation> animation)
    {
        animations[key] = std::move(animation);
    }

    void AnimationComponent::play(const std::string& key, float dt)
    {
        animations[key]->play(dt);
    }
}