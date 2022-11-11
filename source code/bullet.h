#pragma once
#include "includes.h"

class BloonManager;
class Bloon;

class bullet{
private:
	BloonManager* bloons;
	sf::Vector2f WindowSize;
	const float Speed;
	const sf::Vector2f Dir;
	const int MaxBloonsHit;
	const float dmg;
	bool needDelete;
	std::vector<Bloon*> hited;
	sf::Vector2f Pos;
	const sf::Vector2f Size;
	const float angle;
	int* BloonsPoped;

public:
	bullet(const sf::Vector2f Size, BloonManager* bloons, sf::Vector2f WindowSize,
		sf::Vector2f StartPos, sf::Vector2f Dir, const float& Speed, const float& dmg, const int& MaxHits, const float& angle, int* BloonsPoped);

	void updateMovement(const float& dt);
	void updateBullet(Bloon* bloon);

	const sf::Vector2f getDrawPos() { return { Pos.x - Size.x / 2.f, Pos.y - Size.y / 2.f }; };
	const bool& getNeedDelete() const { return needDelete; };
	const float& getAngle() const { return angle; };
};

