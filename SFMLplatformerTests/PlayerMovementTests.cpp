#include "pch.h"
#include <gtest/gtest.h>
#include "ConfigManager.h"
#include "Player.h"

TEST(PlayerMovement, MoveRightTwoTiles)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;

    player.setPosition(0.f, 0.f);

    const float dt = 1.f / 60.f;

    while (player.getPosition().x < 64.f)
    {
        player.move(1.f, 0.f, dt);
        player.update(dt);
    }

    EXPECT_NEAR(player.getPosition().x, 64.f, 3.f);
}

TEST(PlayerMovement, InvulnerablePlayerMovesFaster)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player normalPlayer;
    fp::Player fastPlayer;

    normalPlayer.setPosition(0.f, 0.f);
    fastPlayer.setPosition(0.f, 0.f);

    fastPlayer.activateInvulnerability();

    const float dt = 1.f / 60.f;

    for (int i = 0; i < 60; i++)
    {
        normalPlayer.move(1.f, 0.f, dt);
        normalPlayer.update(dt);

        fastPlayer.move(1.f, 0.f, dt);
        fastPlayer.update(dt);
    }

    EXPECT_GT(
        fastPlayer.getPosition().x,
        normalPlayer.getPosition().x
    );
}

TEST(PlayerMovement, MoveRightFourTiles)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;

    player.setPosition(0.f, 0.f);

    const float dt = 1.f / 60.f;

    while (player.getPosition().x < 128.f)
    {
        player.move(1.f, 0.f, dt);
        player.update(dt);
    }

    EXPECT_NEAR(player.getPosition().x, 128.f, 3.f);
}