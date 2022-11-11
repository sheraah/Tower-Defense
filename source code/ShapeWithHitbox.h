#pragma once
#include<SFML/Graphics.hpp>
#include "sfLine.h"
#include<iostream>
class ShapeWithHitbox{
private:
	sf::FloatRect Hitbox;
	sf::RectangleShape Shape;
	sf::Texture texture;

public:
	ShapeWithHitbox() { };
	ShapeWithHitbox(const sf::FloatRect& Hitbox, const std::string& TexPath, const sf::Vector2f& Size);

	void drawHitbox(sf::RenderTarget* window, const sf::Color& color, const float& Thickness);
	void render(sf::RenderTarget* window);

	const sf::Vector2f getPos() const { return sf::Vector2f(Shape.getPosition().x + Hitbox.left, Shape.getPosition().y + Hitbox.top); }
	const sf::FloatRect getBounds() const;
	const sf::Vector2f getSize() const;
	const sf::Texture& getTexture() const { return texture; };
	const float getRotation() const { return Shape.getRotation(); };

	void setHitbox(const sf::FloatRect& Hitbox);
	void setPos(const float& x, const float& y) { Shape.setPosition(x - Hitbox.left, y - Hitbox.top); }
	void setSize(const float& x, const float& y);
	void setTexture(const std::string TexPath);
	void setTexRect(const sf::IntRect rect) { Shape.setTextureRect(rect); }
	void move(const float x, const float y) { Shape.move(x, y); }
	void setOrigin(const float x, const float y) { Shape.setOrigin(x, y); };
	void rotate(const float angle) { 
		const sf::Vector2f& ori = Shape.getOrigin();
		Shape.setOrigin(Shape.getSize().x / 2.f, Shape.getSize().y / 2.f);
		Shape.rotate(angle); 
		Shape.setOrigin(ori);
	};
	const sf::Vector2f& getOriginalSize() const { return Shape.getSize(); };
	const sf::Vector2f getOriginalPos() {
		return { Shape.getPosition().x - Shape.getOrigin().x, Shape.getPosition().y - Shape.getOrigin().y };
	};

};