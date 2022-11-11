#pragma once
#include "bullet.h"

class GameState;
class Bloon;
class Timer;


class Tower {
private:
	GameState* game;
	
	ShapeWithHitbox shape;
	ShapeWithHitbox BulletShape;
	sf::CircleShape AttackRadius;
	sf::Color CanPlaceColor;
	sf::Color CantPlaceColor;
	int Level;
	int MaxLevel;
	int sellPrice;
	bool isPlaced;
	bool canPlace;
	bool onTowerInfo;
	bool MouseHeld;
	Timer* bulletTimer;
	bool needUpdate;
	size_t AngleIndex;
	int BloonsPoped;

	Settings::Tower::TowerInfo TowerInfo;
	std::vector<std::unique_ptr<bullet>> bullets;
	void initTower();

	void updateTower(const float& dt);
	void renderTower();
public:
	Tower(const int TowerInfoIndex, GameState* game);

	void update(const float& dt);
	void render();
	virtual ~Tower() { delete bulletTimer; bulletTimer = nullptr; }
	void upgrade();
	void updateTowerCollision(const size_t& index);
	void checkForTarget(Bloon* bloon);
	void setDeleted() { canPlace = false; };
	void drawBullet(const sf::Vector2f& Pos);
	bool needDelete() { return isPlaced && !canPlace; };
	const bool& GetOnTowerInfo() const { return onTowerInfo; };
	const int& getLevel() const { return Level; };
	const int& getMaxLevel() const { return MaxLevel; };
	const int getUpgradeCost() const { if (Level >= MaxLevel) return 0; return TowerInfo.upgradeCosts[Level - 1]; };
	const int& getTowerPrice() const { return TowerInfo.TowerPrice; };
	const sf::FloatRect getBounds() const { return shape.getBounds(); };
	const int getSellPrice() const { return sellPrice; };
	const float& getDmg() const { return TowerInfo.Dmg; };
	const float& getShootTimer() const { return bulletTimer->getTimerMax(); };
	const float& getDmgMult() const { return TowerInfo.DmgMultPerLevel; };
	const float& getBulletSpeedMult() const { return TowerInfo.ShootingSpeedMultPerLevel; };
	const int& getBloonsPoped() const { return BloonsPoped; };
	std::vector<std::unique_ptr<bullet>>& getBullets() { return bullets; };
};

