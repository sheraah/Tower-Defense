#include "bullet.h"
#include "BloonManager.h"

bullet::bullet(const sf::Vector2f Size, BloonManager* bloons, sf::Vector2f WindowSize,
	sf::Vector2f StartPos, sf::Vector2f Dir, const float& Speed, const float& dmg, 
	const int& MaxBloonsHit, const float& angle, int* BloonsPoped) :
	bloons(bloons), Size(Size), Speed(Speed), WindowSize(WindowSize), Dir(Dir), MaxBloonsHit(MaxBloonsHit), 
	dmg(dmg), Pos(StartPos), angle(angle), BloonsPoped(BloonsPoped) {
}

void bullet::updateMovement(const float& dt) {
	Pos.x += Dir.x * Speed * dt;
	Pos.y += Dir.y * Speed * dt;
}

void bullet::updateBullet(Bloon* bloon){
	bool wasnthit = true;

	for (const auto& B : hited) { if (B == bloon) wasnthit = false; };

	if (wasnthit) {
		if (sf::FloatRect(Pos.x - Size.x / 2.f, Pos.y - Size.y / 2.f, Size.x, Size.y).intersects(bloon->getBounds())) {
			bloon->takeDmg(dmg, BloonsPoped);
			hited.push_back(bloon);

			if (hited.size() >= MaxBloonsHit) {
				needDelete = true;
				return;
			}
		}
	}

	if (Pos.x < -Size.x ||
		Pos.x > WindowSize.x ||
		Pos.y < -Size.y ||
		Pos.y > WindowSize.y) {
		needDelete = true;
		return;
	}
}
