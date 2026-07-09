#include "pch.h"
#include <gtest/gtest.h>
#include "ConfigManager.h"
#include "Player.h"

TEST(PlayerHealth, DamageHalf)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;

    player.takeDamage(player.getMaxHealth() / 2.f);

    EXPECT_EQ(player.getHealth(), 1);
    EXPECT_TRUE(player.isAlive());
}

TEST(PlayerHealth, Death)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;

    player.loseLife(player.getStartLives());

    EXPECT_EQ(player.getLives(), 0);
    EXPECT_FALSE(player.isAlive());
}

TEST(PlayerHealth, Heal)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;

    player.addLife(1);
    player.takeDamage(player.getMaxHealth() / 2.f);
    player.loseLife(player.getStartLives());

    player.heal();

    EXPECT_EQ(player.getLives(), 1);
    EXPECT_EQ(player.getHealth(), player.getMaxHealth());
    EXPECT_TRUE(player.isAlive());
}