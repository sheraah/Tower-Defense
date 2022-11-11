#pragma once
#include "UI.h"

class Bloon
{
private:
	SoundPlayer* PopSound;
	std::vector<std::unique_ptr<ShapeWithHitbox>>* BloonShapes;
	
	float TraveledDist;
	int BloonInfoIndex;
	const int FirstBloonType;
	bool dead;
	bool OverStart;
	size_t BloonWayIndex;
	sf::Vector2f Pos;
	Settings::Bloon::BloonInfo CurrentBloon;

	void initBloonType(const int index);
public:
	Bloon(std::vector<std::unique_ptr<ShapeWithHitbox>>* BloonShapes, const int BloonInfoIndex, const sf::Vector2f& Pos, SoundPlayer* PopSound);

	void move(const float& dt, const sf::Vector2i& Dir, const float& Dist, const sf::Vector2u& WindowSize);
	void render(sf::RenderWindow* window);
	void takeDmg(const float& Amount, int* BloonsPoped);
	const bool isDead() const;
	const sf::Vector2f getPos() const;
	const size_t& getBloonWayIndex() const;
	const int& getDmg() const;
	const sf::Vector2f getSize() const;
	const bool isOverStart() const;
	const sf::FloatRect getBounds() const;
	const int& getFirstBloonType() const;
};

