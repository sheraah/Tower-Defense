#pragma once
#include<SFML/Graphics.hpp>

class HPBar{
private:
	float MaxValue;
	float* Value;
	sf::RectangleShape FrontBar;
	sf::RectangleShape BackBar;
	

public:
	HPBar(float* Value, const float& MaxValue, const sf::Vector2f& Size, const sf::Vector2f& Pos,
		const sf::Color& FrontColor = sf::Color::Green, const sf::Color& BackColor = sf::Color::Red);

	void render(sf::RenderWindow* window);
	void setPos(const sf::Vector2f& Pos);

	const sf::Vector2f getSize() const;
};

