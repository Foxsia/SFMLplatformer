#include "CameraController.h"

#include "Player.h"
#include "TileMap.h"

namespace fp
{
    void CameraController::followPlayer(sf::View& camera,
        const Player& player,
        const TileMap& map
    )
    {
		sf::FloatRect playerBounds = player.getGlobalBounds();

		float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

		float cameraCenterX = camera.getCenter().x;

		float halfWidth = camera.getSize().x / 2.f;

		// deadzone
		float rightBorder = cameraCenterX + (camera.getSize().x / 2.f);

		float leftBorder = cameraCenterX - (camera.getSize().x / 2.f);

		// move camera right
		if (playerCenterX > rightBorder)
		{
			cameraCenterX = playerCenterX - (camera.getSize().x / 2.f);
		}

		// move camera left
		if (playerCenterX < leftBorder)
		{
			cameraCenterX = playerCenterX + (camera.getSize().x / 2.f);
		}

		// map bounds
		float mapWidth = map.getWidth() * map.getTileSize();

		if (cameraCenterX < halfWidth) cameraCenterX = halfWidth;

		if (cameraCenterX > mapWidth - halfWidth) cameraCenterX = mapWidth - halfWidth;

		camera.setCenter(
			cameraCenterX,
			camera.getSize().y / 2.f
		);
    }
}