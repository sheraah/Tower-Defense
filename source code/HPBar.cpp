#include "HPBar.h"
//#include<iostream>

HPBar::HPBar(float* Value, const float& MaxValue, const sf::Vector2f& Size, const sf::Vector2f& Pos,
	const sf::Color& FrontColor, const sf::Color& BackColor) : MaxValue(MaxValue) {

	this->Value = Value;

	FrontBar.setPosition(Pos);
	FrontBar.setSize(Size);
	FrontBar.setFillColor(FrontColor);
	BackBar.setPosition(Pos);
	BackBar.setSize(Size);
	BackBar.setFillColor(BackColor);
}

void HPBar::render(sf::RenderWindow* window){
	if(*Value>= 0)
	FrontBar.setSize({ BackBar.getSize().x * (*Value / MaxValue) ,BackBar.getSize().y });

	//std::cout << *Value << std::endl;

	window->draw(BackBar);
	window->draw(FrontBar);
}

void HPBar::setPos(const sf::Vector2f& Pos){
	FrontBar.setPosition(Pos);
	BackBar.setPosition(Pos);
}

const sf::Vector2f HPBar::getSize() const{
	return BackBar.getSize();
}
