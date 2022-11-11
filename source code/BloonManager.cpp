#include "BloonManager.h"
#include "GameState.h"

BloonManager::BloonManager(GameState* game): game(game) {
	initVariables();
	initBloons();
}

void BloonManager::initNextRound(){
	BloonSpawns.clear();
	CurrentRound++;
	initBloonSpawns();
}

void BloonManager::initBloons() {
	PopSound = std::unique_ptr<SoundPlayer>(new SoundPlayer(Settings::Game::PopSoundPath, Settings::Game::PopSoundVolume, 1.f));

	for (size_t i = 0; i < Settings::Bloon::Bloons.size(); i++) {
		BloonShapes.push_back(std::unique_ptr<ShapeWithHitbox>(new ShapeWithHitbox()));
		float HitboxValue = (Settings::Bloon::Bloons[i].BloonSize.x / 5.f + Settings::Bloon::Bloons[i].BloonSize.y / 5.f) / 2.f;
		BloonShapes[i]->setSize(Settings::Bloon::Bloons[i].BloonSize.x, Settings::Bloon::Bloons[i].BloonSize.y);
		BloonShapes[i]->setHitbox({ HitboxValue, HitboxValue, Settings::Bloon::Bloons[i].BloonSize.x - HitboxValue * 2.f,
			Settings::Bloon::Bloons[i].BloonSize.y - HitboxValue * 2.f });
		BloonShapes[i]->setTexture(Settings::Bloon::Bloons[i].TexturePath);
	}
}

void BloonManager::initVariables(){
	CurrentRound = 0;
	roundEnd = true;
}

void BloonManager::initBloonSpawns(){
	std::vector<std::string> file;

	try {
		file = game->func->ReadFromFile("txt/Level1/round" + std::to_string(CurrentRound) + ".txt");
	}
	catch (...) {
		game->MissionCompleted = true;
		return;
	}
	BloonSpawnerIndex = 0;
	for (const auto& F : file) {
		std::string Count = "";
		std::string Type = "";
		std::string Timer = "";
		for (const auto& f : F) {
			if (f == ' ') continue;

			if (Type == "") if (game->func->isCharNumber(f)) { Count += f; continue; }
			if (Type != "") if (game->func->isCharNumber(f) || f == '.') { Timer += f; continue; }
			Type += f;		
		}

		if (Count == "" || Type == "" || Timer == "") continue;

		int index = -1;
		for (size_t i = 0; i < Settings::Bloon::Bloons.size(); i++) {
			if (Type == Settings::Bloon::Bloons[i].Name) {
				index = i;
				break;
			}
		}
		if (index == -1) throw "No bloon with that string found";

		for(size_t i = 0; i <= game->func->StrToNum<size_t>(Count); i++)
		BloonSpawns.push_back({ index,  game->func->StrToNum<float>(Timer) });
	}

	BloonSpawnTimer = Timer(BloonSpawns[BloonSpawnerIndex].Timer);
	roundEnd = false;
}

bool BloonManager::updateBloons(const float& dt, const size_t& i) {
	    PopSound->update(dt);
		Bloons[i]->move(dt, game->Map->getBloonTravelWay()[Bloons[i]->getBloonWayIndex()].Dir,
			game->Map->getBloonTravelWay()[Bloons[i]->getBloonWayIndex()].distance, game->window->getSize());

		if (Bloons[i]->isOverStart() && (Bloons[i]->getPos().x < -Bloons[i]->getSize().x ||
			Bloons[i]->getPos().x > game->window->getSize().x || Bloons[i]->getPos().y < -Bloons[i]->getSize().y ||
			Bloons[i]->getPos().y > game->window->getSize().y)) {
			game->Hp -= Bloons[i]->getDmg();
			Bloons[i].reset();
			Bloons.erase(Bloons.begin() + i);
			return true;
		}
		
		if (Bloons[i]->isDead()) {
			int r = rand() % 2;
			if (r == 1) {
				game->incomeMoney += Bloons[i]->getFirstBloonType();
			}
			Bloons[i].reset();
			Bloons.erase(Bloons.begin() + i);
			PopSound->play();
			return true;
		}

		return false;
}


void BloonManager::updateBloonSpawns(const float& dt){
	if (roundEnd) return;

	if (BloonSpawnTimer.timerOver(dt)) {
		if (BloonSpawnerIndex + 1 >= BloonSpawns.size()) { roundEnd = true; return; }
		BloonSpawnerIndex++;

		Bloons.push_back(std::unique_ptr<Bloon>(new Bloon(&BloonShapes, BloonSpawns[BloonSpawnerIndex - 1].BloonType, game->Map->getBloonStartPos(), PopSound.get())));

		BloonSpawnTimer.setTimerMax(BloonSpawns[BloonSpawnerIndex].Timer);
	}
}

void BloonManager::update(const float& dt){
	updateBloonSpawns(dt);
}

void BloonManager::render(){
	for (const auto& B : Bloons) {
		B->render(game->window);
	}
}

