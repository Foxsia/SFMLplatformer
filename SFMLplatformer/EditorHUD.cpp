#include "EditorHUD.h"

namespace fp
{
    namespace
    {
        const unsigned HELP_TEXT_SIZE = 15;
        const float HELP_TEXT_X = 10.f;
        const float HELP_TEXT_Y = 10.f;
    }
    EditorHUD::EditorHUD()
    {
        tileTexture.loadFromFile("assets/world_tileset.png");

        playerTexture.loadFromFile("assets/player.png");
        enemyTexture.loadFromFile("assets/slime_green.png");
        lifeTexture.loadFromFile("assets/fruit.png");
        fireTexture.loadFromFile("assets/fire_fruit.png");
        goalTexture.loadFromFile("assets/end_mushroom.png");
        invulnerabilityTexture.loadFromFile("assets/flash_fruit.png");
        fireEnemyTexture.loadFromFile("assets/slime_purple.png");
    }
    void EditorHUD::render(sf::RenderWindow& window, const sf::View& camera, const sf::Font& font, BrushType currentBrush)
	{
        sf::Text editorHelp;
        editorHelp.setFont(font);
        editorHelp.setCharacterSize(HELP_TEXT_SIZE);
        editorHelp.setFillColor(sf::Color::White);
        editorHelp.setPosition(
            camera.getCenter().x - camera.getSize().x / 2.f + HELP_TEXT_X,
            camera.getCenter().y - camera.getSize().y / 2.f + HELP_TEXT_Y
        );

        if (sf::Joystick::isConnected(0))
        {
            editorHelp.setString(
                "Press left joystick to save and right joystick to proceed or triangle to cancel\nPress x to open menu\nPLAYER SPAWN POINT IS REQUIRED!"
            );
        }
        else
        {
            editorHelp.setString(
                "Press F2 to save and Enter to proceed or C to cancel\nPress M to open menu\nPLAYER SPAWN POINT IS REQUIRED!"
            );
        }
        window.draw(editorHelp);

        const float width = 180.f;
        const float height = 120.f;

        sf::Vector2f pos(
            camera.getCenter().x + camera.getSize().x / 2.f - width - 10.f,
            camera.getCenter().y - camera.getSize().y / 2.f + 10.f
        );

        sf::RectangleShape background;
        background.setSize({ width, height });
        background.setPosition(pos);
        background.setFillColor(sf::Color(0, 0, 0, 180));
        background.setOutlineThickness(2.f);
        background.setOutlineColor(sf::Color::White);

        window.draw(background);

        sf::Text controls;
        controls.setFont(font);
        controls.setCharacterSize(20);
        controls.setFillColor(sf::Color::White);

        if (sf::Joystick::isConnected(0))
        {
            controls.setString(
                "LB - prev brush\n"
                "RB - next brush"
            );
        }
        else
        {
            controls.setString(
                "Q - prev brush\n"
                "E - next brush"
            );
        }

        controls.setPosition(pos.x + 50.f, pos.y + 10.f);

        window.draw(controls);

        sf::Text brushText;
        brushText.setFont(font);
        brushText.setCharacterSize(16);
        brushText.setFillColor(sf::Color::Yellow);

        brushText.setString(
            brushToString(currentBrush)
        );

        brushText.setPosition(pos.x + 15.f, pos.y + 80.f);

        window.draw(brushText);

        switch (currentBrush)
        {
        case BrushType::Player:
            brushSprite.setTexture(playerTexture, true);
            brushSprite.setTextureRect(sf::IntRect(9, 9, 13, 19));
            brushSprite.setScale(2.f, 2.f);
            break;

        case BrushType::Slime:
            brushSprite.setTexture(enemyTexture, true);
            brushSprite.setTextureRect(sf::IntRect(77, 5, 14, 18));
            brushSprite.setScale(2.f, 2.f);
            break;

        case BrushType::FireEnemy:
            brushSprite.setTexture(fireEnemyTexture, true);
            brushSprite.setTextureRect(sf::IntRect(77, 5, 14, 18));
            brushSprite.setScale(2.f, 2.f);
            break;

        case BrushType::LifeFruit:
            brushSprite.setTexture(lifeTexture, true);
            brushSprite.setScale(1.f, 1.f);
            break;

        case BrushType::FireFruit:
            brushSprite.setTexture(fireTexture, true);
            brushSprite.setScale(1.f, 1.f);
            break;

        case BrushType::Goal:
            brushSprite.setTexture(goalTexture, true);
            brushSprite.setScale(1.f, 1.f);
            break;

        case BrushType::Tile:
            brushSprite.setTexture(tileTexture, true);
            brushSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            brushSprite.setScale(1.f, 1.f);
            break;

        case BrushType::MovingTile:
            brushSprite.setTexture(tileTexture, true);
            brushSprite.setTextureRect(sf::IntRect(128, 0, 32, 32));
            brushSprite.setScale(1.f, 1.f);
            break;
        case BrushType::Portal:
            brushSprite.setTexture(tileTexture, true);
            brushSprite.setTextureRect(sf::IntRect(0, 320, 32, 32));
            brushSprite.setScale(1.f, 1.f);
            break;
        case BrushType::InvulnerabilityFruit:
            brushSprite.setTexture(invulnerabilityTexture, true);
            brushSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            brushSprite.setScale(1.f, 1.f);
            break;
        }
        brushSprite.setPosition(pos.x + 110.f, pos.y + 70.f);

        window.draw(brushSprite);

	}
	std::string EditorHUD::brushToString(BrushType brush) const
	{
        switch (brush)
        {
            case BrushType::Player: return "Player";
            case BrushType::Tile: return "Tile";
            case BrushType::Slime: return "Slime";
            case BrushType::LifeFruit: return "Life";
            case BrushType::FireFruit: return "Fire";
            case BrushType::Goal: return "Goal";
            case BrushType::MovingTile: return "Moving";
            case BrushType::Portal: return "Portal";
            case BrushType::InvulnerabilityFruit: return "Invulnerability";
            case BrushType::FireEnemy: return "FireEnemy";
            default: return "Unknown";
        }
	}
}
