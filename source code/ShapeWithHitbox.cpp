#include "ShapeWithHitbox.h"



ShapeWithHitbox::ShapeWithHitbox(const sf::FloatRect& Hitbox, const std::string& TexPath, const sf::Vector2f& Size){
	setHitbox(Hitbox);
	setSize(Size.x, Size.y);
	setTexture(TexPath);
}

void ShapeWithHitbox::drawHitbox(sf::RenderTarget* window, const sf::Color& color, const float& Thickness){
	sf::Vector2f TL = { getPos().x - Shape.getOrigin().x,  getPos().y - Shape.getOrigin().y };
	sf::Vector2f TR = { TL.x + getBounds().width, TL.y };
	sf::Vector2f BL = { TL.x, TL.y + getBounds().height };
	sf::Vector2f BR = { TR.x, BL.y };

	window->draw(sfLine(TL, TR, color, Thickness));
	window->draw(sfLine(TR, BR, color, Thickness));
	window->draw(sfLine(BR, BL, color, Thickness));
	window->draw(sfLine(BL, TL, color, Thickness));
}

void ShapeWithHitbox::render(sf::RenderTarget* window){
	window->draw(Shape);
}

const sf::FloatRect ShapeWithHitbox::getBounds() const{
	sf::FloatRect rect = Shape.getGlobalBounds();
	return sf::FloatRect(rect.left + Hitbox.left, rect.top + Hitbox.top, rect.width - (rect.width - Hitbox.width), 
		rect.height - (rect.height - Hitbox.height));
}

const sf::Vector2f ShapeWithHitbox::getSize() const
{
	sf::FloatRect Bounds = getBounds();
	return sf::Vector2f(Bounds.width, Bounds.height);
}

void ShapeWithHitbox::setHitbox(const sf::FloatRect& Hitbox){
	this->Hitbox = Hitbox;
}

void ShapeWithHitbox::setSize(const float& x, const float& y){
	Shape.setSize({ x, y });
	Hitbox = { 0, 0, x, y };
} 

void ShapeWithHitbox::setTexture(const std::string TexPath){
	texture.loadFromFile(TexPath);
	Shape.setTexture(&texture);
}