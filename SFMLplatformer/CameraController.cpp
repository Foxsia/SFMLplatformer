#include "CameraController.h"

#include "Player.h"
#include "TileMap.h"
#include "InputManager.h"

namespace fp
{
    void CameraController::followPlayer(sf::View& camera,
        const Player& player,
        const TileMap& map
    )
    {
		const sf::FloatRect playerBounds = player.getGlobalBounds();

		const float playerCenterX = playerBounds.left + playerBounds.width / 2.f;

		float cameraCenterX = camera.getCenter().x;

		const float halfWidth = camera.getSize().x / 2.f;

		const float rightBorder = cameraCenterX + (camera.getSize().x / 2.f);

		const float leftBorder = cameraCenterX - (camera.getSize().x / 2.f);

		if (playerCenterX > rightBorder)
		{
			cameraCenterX = playerCenterX - (camera.getSize().x / 2.f);
		}

		if (playerCenterX < leftBorder)
		{
			cameraCenterX = playerCenterX + (camera.getSize().x / 2.f);
		}

		const float mapWidth = map.getWidth() * map.getTileSize();

		if (cameraCenterX < halfWidth) cameraCenterX = halfWidth;

		if (cameraCenterX > mapWidth - halfWidth) cameraCenterX = mapWidth - halfWidth;

		camera.setCenter(cameraCenterX, camera.getSize().y / 2.f);
    }
	void CameraController::moveEditor(sf::View& camera, const InputManager& input, const TileMap& map)
	{
		const float cameraSpeed = 5.f;

		float cameraCenterX = camera.getCenter().x;

		if (input.isActionPressed("MOVE_LEFT"))
		{
			cameraCenterX -= cameraSpeed;
		}

		if (input.isActionPressed("MOVE_RIGHT"))
		{
			cameraCenterX += cameraSpeed;
		}

		const float mapWidth = map.getWidth() * map.getTileSize();
		const float halfWidth = camera.getSize().x / 2.f;

		if (cameraCenterX < halfWidth)
		{
			cameraCenterX = halfWidth;
		}

		if (cameraCenterX > mapWidth - halfWidth)
		{
			cameraCenterX = mapWidth - halfWidth;
		}

		camera.setCenter(cameraCenterX, camera.getCenter().y);
	}
}