#include "Bloon.h"

Bloon::Bloon(std::vector<std::unique_ptr<ShapeWithHitbox>>* BloonShapes,const int BloonInfoIndex, const sf::Vector2f& Pos, SoundPlayer* PopSound):
	BloonShapes(BloonShapes), PopSound(PopSound), CurrentBloon(Settings::Bloon::Bloons[BloonInfoIndex]), FirstBloonType(BloonInfoIndex + 1) {
	this->Pos = Pos;
	this->BloonInfoIndex = BloonInfoIndex;

	TraveledDist = 0;
	BloonWayIndex = 0;
	dead = false;
	OverStart = false;
}

void Bloon::initBloonType(const int index){
	CurrentBloon = Settings::Bloon::Bloons[index];
}
void Bloon::move(const float& dt, const sf::Vector2i& Dir, const float& Dist, const sf::Vector2u& WindowSize){
	if (!OverStart) {
		if(!(getPos().x < 0 || getPos().x + getSize().x > WindowSize.x ||
			getPos().y < 0 || getPos().y + getSize().y > WindowSize.y))
		    OverStart = true;
	}

	sf::Vector2f CurrentSpeed = { Dir.x *  CurrentBloon.Speed * dt, Dir.y * CurrentBloon.Speed * dt };
	Pos.x += CurrentSpeed.x;
	Pos.y += CurrentSpeed.y;

	TraveledDist += abs(CurrentSpeed.x);
	TraveledDist += abs(CurrentSpeed.y);

	if (TraveledDist > Dist) {
		float MoveValue = TraveledDist - Dist;

		if (Dir.x == 0) {
			Pos = sf::Vector2f(Pos.x, Pos.y - MoveValue);
		}
		else {
			Pos = sf::Vector2f(Pos.x - MoveValue, Pos.y);
		}

		TraveledDist = 0;
		BloonWayIndex++;
	}
}


void Bloon::takeDmg(const float& Amount, int* BloonsPoped) {
	if (dead) return;
	
	if(CurrentBloon.Hp - Amount <= 0){
		(*BloonsPoped)++;
		float PrevHP = CurrentBloon.Hp;
		BloonInfoIndex--;
		if (BloonInfoIndex < 0) {
			dead = true;
			BloonInfoIndex = 0;
			return;
		}
		PopSound->play();
		initBloonType(BloonInfoIndex);
		return takeDmg(Amount - PrevHP, BloonsPoped);
	}
	
	CurrentBloon.Hp -= Amount;
}

const bool Bloon::isDead() const{
	return dead;
}

const sf::Vector2f Bloon::getPos() const{
	return Pos;
}

const size_t& Bloon::getBloonWayIndex() const{
	return BloonWayIndex;
}

const int& Bloon::getDmg() const{
	return CurrentBloon.Dmg;
}

const sf::Vector2f Bloon::getSize() const {
	return CurrentBloon.BloonSize;
}
const bool Bloon::isOverStart() const {
	return OverStart;
}

const sf::FloatRect Bloon::getBounds() const {
	return { Pos.x - BloonShapes->at(BloonInfoIndex)->getSize().x / 2.f, Pos.y - BloonShapes->at(BloonInfoIndex)->getSize().y / 2.f,
			  BloonShapes->at(BloonInfoIndex)->getSize().x, BloonShapes->at(BloonInfoIndex)->getSize().y };
}

const int& Bloon::getFirstBloonType() const{
	return FirstBloonType;
}

void Bloon::render(sf::RenderWindow* window){
	if (dead) return;
	BloonShapes->at(BloonInfoIndex)->setPos(Pos.x - BloonShapes->at(BloonInfoIndex)->getSize().x / 2.f,
		Pos.y - BloonShapes->at(BloonInfoIndex)->getSize().y / 2.f);
	BloonShapes->at(BloonInfoIndex)->render(window);
}
