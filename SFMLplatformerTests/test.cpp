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