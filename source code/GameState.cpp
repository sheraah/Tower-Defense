#include "GameState.h"

GameState::GameState(sf::RenderWindow* window): window(window) {
	func = std::unique_ptr<Func>(new Func(window));
	initMap();
	initVariables();
	initObjects();
	initGui();
}

void GameState::initVariables(){
	font.loadFromFile("Fonts/Stan.ttf");
	Hp = MaxHp;
	music = std::unique_ptr<sf::Music>(new sf::Music());
	music->openFromFile(Settings::Game::GameMusicSoundPath);
	music->setVolume(Settings::Game::GameMusicVolume);
	music->setLoop(true);
	music->play();

	PlaceSoundbuffer.loadFromFile(Settings::Game::PlaceSoundPath);
	PlaceSound.setBuffer(PlaceSoundbuffer);
	PlaceSound.setVolume(Settings::Game::PlaceSoundVolume);


	SellSoundbuffer.loadFromFile(Settings::Game::SellSoundPath);
	SellSound.setBuffer(SellSoundbuffer);
	SellSound.setVolume(Settings::Game::SellSoundVolume);

	LevelUpSoundbuffer.loadFromFile(Settings::Game::LevelUpSoundPath);
	LevelUpSound.setBuffer(LevelUpSoundbuffer);
	LevelUpSound.setVolume(Settings::Game::LevelUpSoundVolume);

	incomeTimer = Timer(Settings::Game::IncomeTimer);
	incomeTimer.setReversed(true);
	incomeMoney = Settings::Game::IncomeMoneyStart;

	Dead = false;
	MissionCompleted = false;
}

void GameState::initMap() {
	Map = std::unique_ptr<TileMap>(new TileMap(func.get()));
}


void GameState::initGui() {
	ui = std::unique_ptr<UI>(new UI(this));
}

void GameState::initObjects(){
	Bloons = std::unique_ptr<BloonManager>(new BloonManager(this));
	money = std::unique_ptr<Money>(new Money(Settings::Game::StartMoney));
}

void GameState::pollEvents() {
	while (window->pollEvent(evnt)) {
		switch (evnt.type) {
		case sf::Event::Closed:
			window->close();
			break;
		}
	}
}

void GameState::updateMouse(){
	MousePos = sf::Mouse::getPosition(*window);
	if (MousePos.x < 0) MousePos.x = 0;
	if (MousePos.y < 0) MousePos.y = 0;
	if (MousePos.x > window->getSize().x) MousePos.x = window->getSize().x;
	if (MousePos.y > window->getSize().y) MousePos.y = window->getSize().y;
}

void GameState::updateBloons(const float& dt){
	if (ui->isPrepPhaseOver(dt)){
		Bloons->initNextRound();
	}

	Bloons->update(dt);
}

void GameState::updateTowers(const float& dt) {
	if (Towers.size() > 0) {
		bool TowerSelected = false;
		for (size_t i = 0; i < Towers.size(); i++) {
			Towers[i]->update(dt);
			Towers[i]->updateTowerCollision(i);
			if (Towers[i]->needDelete()) {
				Towers[i].reset();
				Towers.erase(Towers.begin() + i);
				continue;
			}
			if (Bloons->getBloons().size() > 0) {
				for (size_t k = 0; k < Bloons->getBloons().size(); k++) {
					Bloon* bloon = Bloons->getBloons()[k].get();
					Towers[i]->checkForTarget(bloon);
					if (i == Towers.size() - 1) {
						if (Bloons->updateBloons(dt, k)) continue;
					}

					for (size_t j = 0; j < Towers[i]->getBullets().size(); j++) {
						Towers[i]->getBullets()[j]->updateBullet(bloon);

						if (k == Bloons->getBloons().size() - 1) {
							Towers[i]->getBullets()[j]->updateMovement(dt);
						}

						if (Towers[i]->getBullets()[j]->getNeedDelete()) {
							Towers[i]->getBullets()[j].reset();
							Towers[i]->getBullets().erase(Towers[i]->getBullets().begin() + j);
						}
					}
				}
			}
			else {
				for (size_t j = 0; j < Towers[i]->getBullets().size(); j++) {
					Towers[i]->getBullets()[j]->updateMovement(dt);
				}
			}

			if (Towers[i]->GetOnTowerInfo()) {
				TowerSelected = true;
				if (!ui->getSelectedTower()) ui->setSelectedTower(Towers[i].get());
			}
		}

		if (!TowerSelected) ui->setSelectedTower(nullptr);
	}
	else {
		for (size_t k = 0; k < Bloons->getBloons().size(); k++) {
			Bloons->updateBloons(dt, k);
		}
	}
}

void GameState::renderTowers(){
	for (const auto& T : Towers) {
		T->render();
	}
}

void GameState::update(const float& dt){
	fps = 1.f / dt;

	pollEvents();
	updateMouse();

    ui->update();
	if (Dead || MissionCompleted) return;
	if (ui->getPause()) return;
	
    updateTowers(dt);

	updateBloons(dt);

	if (incomeTimer.timerOver(dt)) {
		money->Give(incomeMoney);
	}

	if (Hp <= 0) Dead = true;
}



void GameState::draw(){
	Map->render();

	Bloons->render();
	
 	renderTowers();
	
	ui->render();
}
