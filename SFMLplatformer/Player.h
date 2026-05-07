#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;

	//animation
	sf::IntRect currentFrame;

	void initTexture();
	void initSprite();
public:
	Player();
	virtual ~Player();

	//func
	void updateMovement();
	void update();
	void render(sf::RenderTarget& target);
};

