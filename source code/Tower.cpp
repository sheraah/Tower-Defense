#include "Tower.h"
#include "GameState.h"

Tower::Tower(const int TowerInfoIndex, GameState* game) :game(game), AngleIndex(0), TowerInfo(Settings::Tower::Towers[TowerInfoIndex]) {
	initTower();
}

void Tower::initTower(){
	BloonsPoped = 0;
	sellPrice = static_cast<int>(TowerInfo.TowerPrice * 0.75f);
	isPlaced = false;
	onTowerInfo = false;
	needUpdate = false;
	Level = 1;
	MaxLevel = 1 + TowerInfo.upgradeCosts.size();
    shape.setSize(TowerInfo.TowerSize.x, TowerInfo.TowerSize.y);
	shape.setTexture(TowerInfo.TexturePath + "/LVL" + std::to_string(Level) + ".png");
	shape.setHitbox({ 0, 10, TowerInfo.TowerSize.x, TowerInfo.TowerSize.y - 10 });
	shape.setOrigin(shape.getSize().x / 2.f, shape.getSize().y / 2.f);

	AttackRadius.setRadius(TowerInfo.AttackRadius);
	AttackRadius.setOrigin(TowerInfo.AttackRadius, TowerInfo.AttackRadius);
	CanPlaceColor = sf::Color::Color(0, 0, 0, 100);
	CantPlaceColor = sf::Color::Color(255, 0, 0, 150);
	AttackRadius.setFillColor(CanPlaceColor);

	BulletShape.setTexture(TowerInfo.TexturePath + "/Bullet.png");
	BulletShape.setSize(TowerInfo.BulletSize.x, TowerInfo.BulletSize.y);

	bulletTimer = new Timer(TowerInfo.ShootTimerInSec);
}

void Tower::updateTower(const float& dt){
	if (!isPlaced) {
		shape.setPos(static_cast<float>(game->MousePos.x), static_cast<float>(game->MousePos.y));

		canPlace = game->Map->getCollisionTileByRect(shape.getBounds());
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !MouseHeld) {
		MouseHeld = true;
		onTowerInfo = shape.getBounds().contains(static_cast<sf::Vector2f>(game->MousePos)) ||
			(onTowerInfo && game->ui->getTowerUIBounds().contains(static_cast<sf::Vector2f>(game->MousePos)));
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		MouseHeld = false;
	}

	if (bulletTimer->timerOver(dt) && !needUpdate) {
		needUpdate = true;
	}
}

void Tower::upgrade() {
	if (Level >= MaxLevel) return;
	sellPrice += getUpgradeCost() * 0.75f;
;	Level++;
	shape.setTexture(TowerInfo.TexturePath + "/LVL" + std::to_string(Level) + ".png");
	TowerInfo.Dmg *= TowerInfo.DmgMultPerLevel;
	AttackRadius.setRadius(AttackRadius.getRadius() * TowerInfo.RadiusMultPerLevel);
	AttackRadius.setOrigin(AttackRadius.getRadius(), AttackRadius.getRadius());
	AttackRadius.setPosition(shape.getPos());
	TowerInfo.MaxBloonHit += TowerInfo.MaxBloonHitPerLevel;
	bulletTimer->setTimerMax(bulletTimer->getTimerMax() - bulletTimer->getTimerMax() * TowerInfo.ShootingSpeedMultPerLevel);
	game->LevelUpSound.play();
}

void Tower::updateTowerCollision(const size_t& index){
	if (!isPlaced) {
		for (size_t i = 0; i < game->Towers.size(); i++) {
			if (i == index) continue;

			if (shape.getBounds().intersects(game->Towers[i]->getBounds())) {
				canPlace = false; break;
			}
		}

		canPlace ? AttackRadius.setFillColor(CanPlaceColor) : AttackRadius.setFillColor(CantPlaceColor);
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isPlaced = true;

			if (!needDelete()) {
				game->money->Lose(TowerInfo.TowerPrice);
				game->PlaceSound.play();
			}
		}
	}
}

void Tower::checkForTarget(Bloon* bloon){
	if (!isPlaced) return;

	if (needUpdate) {
		if (game->func->isShapeInCircle(bloon->getBounds(), AttackRadius.getPosition(), AttackRadius.getRadius())) {
			needUpdate = false;
			bulletTimer->reset();
			float angle = game->func->CalcRotationAngle({ shape.getPos() },
				{ bloon->getPos() }, shape.getRotation());

			shape.rotate(angle);

			sf::Vector2f Center = { shape.getOriginalPos().x + shape.getOriginalSize().x / 2.f,
					shape.getOriginalPos().y + shape.getOriginalSize().y / 2.f };
			
			sf::Vector2f StartPos = game->func->getPointOnCircle(Center,
				(shape.getOriginalSize().x * 0.4f + shape.getOriginalSize().y * 0.4f) / 2.f, shape.getRotation() + TowerInfo.ShootingAngles[AngleIndex]);
			
			sf::Vector2f EndPos = game->func->getPointOnCircle(StartPos, 1 , shape.getRotation());

			sf::Vector2f Dir = game->func->NormVector(StartPos, EndPos);

			bullets.push_back(std::unique_ptr<bullet>(new bullet(BulletShape.getSize(), game->Bloons.get(),
				static_cast<sf::Vector2f>(game->window->getSize()), StartPos, Dir , TowerInfo.BulletSpeed, TowerInfo.Dmg,
				static_cast<int>(roundf(TowerInfo.MaxBloonHit)), shape.getRotation(), &BloonsPoped)));
	        
			if (AngleIndex < TowerInfo.ShootingAngles.size() - 1) AngleIndex++;
			else AngleIndex = 0;		
		}	
	}
}

void Tower::drawBullet(const sf::Vector2f& Pos){
	BulletShape.setPos(Pos.x, Pos.y);
	BulletShape.render(game->window);
}

void Tower::renderTower(){
	if (!isPlaced) {
		AttackRadius.setPosition(shape.getPos());
	    game->window->draw(AttackRadius);
		
	}

	if (onTowerInfo) game->window->draw(AttackRadius);
	shape.render(game->window);

	for (const auto& B : bullets) {
		sf::Vector2f Pos = B->getDrawPos();
		BulletShape.setPos(Pos.x, Pos.y);
		BulletShape.rotate((360 - BulletShape.getRotation()) + B->getAngle());
		BulletShape.render(game->window);
	}
}


void Tower::update(const float& dt) {
	updateTower(dt);
}

void Tower::render() {
	renderTower();
}
