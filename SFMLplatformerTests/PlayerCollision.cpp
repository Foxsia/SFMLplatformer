#include "pch.h"
#include <gtest/gtest.h>
#include "ConfigManager.h"
#include "Player.h"
#include "FireEnemy.h"

TEST(CollisionSystem, PlayerOverlapsEnemy)
{
    fp::ConfigManager::load("../SFMLplatformer/config/gameplay.json");

    fp::Player player;
    fp::FireEnemy enemy;

    player.setPosition(100.f, 100.f);
    enemy.setPosition(100.f, 100.f);

    EXPECT_TRUE(
        player.getHitbox().intersects(enemy.getHitbox())
    );
}