#include "UI.h"
#include "GameState.h"

UI::UI(GameState* game) : game(game) {
	initUI();
}

void UI::initUI(){
	HPBackTex.loadFromFile("Textures/HpEmpty.png");
	HPBack.setSize({ 25, 25 });
	HPBack.setTexture(&HPBackTex);
	HPBack.setPosition(850.f, 0.f);

	HPFrontTex.loadFromFile("Textures/HpFull.png");
	HPFront.setSize({ 25, 25 });
	HPFront.setTexture(&HPFrontTex);
	HPFront.setPosition(850.f, 0.f);

	initTowerUI();
	initPrepPhase();
	initTowerInfoUI();
	initText();
}

void UI::initPrepPhase(){
	Pause = true;

	PrepPhaseTimer.setTimerMax(Settings::Game::PrepPhaseTimer);
	PrepPhaseTimer.setReversed(true);

	PlayButton = std::unique_ptr<Button>(new Button(0, 0, 200, 100, &game->font, "PLAY", sf::Color::Black, sf::Color::Black,
		sf::Color::Black, sf::Color::Color(208, 196, 14), sf::Color::Color(243, 229, 10), sf::Color::Color(243, 229, 10),
		sf::Color::Black, sf::Color::Black, sf::Color::Black));

	PlayButton->setPos(game->window->getSize().x / 2 - PlayButton->getBounds().width / 2,
		game->window->getSize().y / 2 - PlayButton->getBounds().height / 2);

	SellButton = std::unique_ptr<Button>(new Button(0, 0, 100, 50, &game->font, "SELL: XXX$", sf::Color::White, sf::Color::White,
		sf::Color::White, sf::Color::Color(255, 0, 0), sf::Color::Color(125, 0, 0), sf::Color::Color(125, 0, 0),
		sf::Color::Black, sf::Color::Black, sf::Color::Black));
    
	SellButton->setPos(TowerSelectionBG.getPosition().x + TowerSelectionBG.getSize().x - SellButton->getSize().x * 1.5f,
		TowerSelectionBG.getPosition().y + TowerSelectionBG.getSize().y - SellButton->getSize().y * 1.5f);

	upgradeButton = std::unique_ptr<Button>(new Button(0, 0, 100, 50, &game->font, "UPGRADE: XXX$", sf::Color::Yellow, sf::Color::Yellow,
		sf::Color::Yellow, sf::Color::Color(7, 147, 247), sf::Color::Color(4, 106, 179), sf::Color::Color(4, 106, 179),
		sf::Color::Black, sf::Color::Black, sf::Color::Black));

	upgradeButton->setPos(SellButton->getPos().x - SellButton->getSize().x - SellButton->getSize().x * 0.2f, 
		SellButton->getPos().y);

	sf::Vector2u FirstRes = game->func->getFirstRes();

	ResCombo = std::unique_ptr<Combo<sf::Vector2u>>(new Combo<sf::Vector2u>("Resolution", &game->font, sf::Color::Yellow,
		Button(100, 100, 100, 50, &game->font, "DANKDWDWWDW", sf::Color::Color(208, 196, 14), sf::Color::Color(243, 229, 10),
			sf::Color::Color(243, 229, 10), sf::Color::Color(0, 0, 0, 200), sf::Color::Color(0, 0, 0, 200), sf::Color::Color(0, 0, 0, 200),
			sf::Color::Black, sf::Color::Black), { {std::to_string(FirstRes.x) + "x" + std::to_string(FirstRes.y), FirstRes} ,{"1920x1080", {1920, 1080}},
		{"1500x1000", {1500, 1000}}, {"800x600", {1500, 1000}} }));
}

void UI::initTowerUI(){
	TowerSelectionBG.setFillColor(sf::Color::Color(164, 116, 73));
	TowerSelectionBG.setSize({ static_cast<float>(game->window->getSize().x),  100});
	TowerSelectionBG.setPosition(static_cast<float>(game->window->getSize().x) / 2.f - TowerSelectionBG.getSize().x / 2.f, 
		static_cast<float>(game->window->getSize().y) - TowerSelectionBG.getSize().y);
	TowerSelectionBG.setOutlineThickness(2.f);
	TowerSelectionBG.setOutlineColor(sf::Color::Black);

	for (size_t i = 0; i < Settings::Tower::Towers.size(); i++) {
		TowersToSelect.push_back(std::unique_ptr<ShapeWithHitbox>(new ShapeWithHitbox()));
		TowersToSelect[i]->setSize(Settings::Tower::Towers[i].TowerSize.x, Settings::Tower::Towers[i].TowerSize.y);
		TowersToSelect[i]->setTexture(Settings::Tower::Towers[i].TexturePath + "/LVL1.png");
	}

	float Dist = 60.f;
	for (size_t i = 0; i < TowersToSelect.size(); i++) {		
		float XPos = i == 0 ? TowerSelectionBG.getPosition().x + Dist * 0.5f : TowersToSelect[i - 1]->getPos().x + Dist * 1.5f;
	
		TowersToSelect[i]->setPos(XPos, TowerSelectionBG.getPosition().y);
	}
}

void UI::initText(){
	Text.setFont(game->font);
	Text.setOutlineThickness(2.f);
	Text.setOutlineColor(sf::Color::Black);
}

void UI::drawText(const std::string str, const unsigned int chSize, const sf::Vector2f Pos, const sf::Color color){
	Text.setString(str);
	Text.setCharacterSize(chSize);
	Text.setPosition(Pos);
	Text.setFillColor(color);
	game->window->draw(Text);
}

void UI::initTowerInfoUI(){

}


void UI::setSelectedTower(Tower* SelectedTower){
	this->SelectedTower = SelectedTower;

	if (!SelectedTower) return;

	SellButton->setText("Sell: " + std::to_string(SelectedTower->getSellPrice()) + "$");
}

void UI::update() {
	updateUI();

	updateText();

	updatePausedScreen();

	updateTowerUI();
}

void UI::updateUI(){
	float Perc = static_cast<float>(game->Hp) / game->MaxHp;

	HPFront.setTextureRect({ 0, 0, static_cast<int>(HPFrontTex.getSize().x * Perc), static_cast<int>(HPFrontTex.getSize().y) });
	HPFront.setSize({ HPBack.getSize().x * Perc, HPBack.getSize().y });
}

void UI::updatePausedScreen(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		Pause = true;
	}

	if (!Pause) return;

	PlayButton->update(game->MousePos);
	ResCombo->update(game->MousePos);


	if (PlayButton->isPressed()) {
		Pause = false;
	}
}

void UI::updateTowerUI() {
	static bool MouseHeld = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !MouseHeld) {
		MouseHeld = true;

		for (size_t i = 0; i < TowersToSelect.size(); i++) {
			if (TowersToSelect[i]->getBounds().contains(static_cast<sf::Vector2f>(game->MousePos))) {
				if (game->money->get() - Settings::Tower::Towers[i].TowerPrice >= 0) {
					game->Towers.push_back(std::unique_ptr<Tower>(new Tower(i, game)));
				}
			}
		}
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		MouseHeld = false;
	}
	if (SelectedTower) {
		if (!SelectedTower->GetOnTowerInfo()) SelectedTower = nullptr;
	}
	
}

void UI::updateText(){

}


void UI::render(){
	drawTowerUI();
	drawText();
	drawPauseScreen();
}

void UI::drawUI(){
	game->window->draw(HPBack);
	game->window->draw(HPFront);
}

void UI::drawText() {
	drawUI();
	drawText(" " + std::to_string(game->Hp) + " / " + std::to_string(game->MaxHp), 15, { HPBack.getPosition().x + HPBack.getSize().x, 0.f }, sf::Color::White);
	drawText("FPS:" + std::to_string(game->fps), 20, {10, 10}, sf::Color::White);
	drawText(std::to_string(game->money->get()) + "$", 20, { HPBack.getPosition().x - 90, 0.f }, sf::Color::Yellow);
	
	if (game->Bloons->getRoundEnd()) {
		drawText(std::to_string(static_cast<int>((PrepPhaseTimer.getTimer()))), 30, { static_cast<float>(game->window->getSize().x / 2 - Text.getCharacterSize()),
		0 }, sf::Color::Yellow);
	}
	else {
		drawText("Round " + std::to_string(game->Bloons->getRound()), 30,
			{ static_cast<float>(game->window->getSize().x / 2) - Text.getGlobalBounds().width / 2, 0 }, sf::Color::Yellow);
	}
}

void UI::drawPauseScreen(){
	if (!Pause) return;
	sf::RectangleShape background(static_cast<sf::Vector2f>(game->window->getSize()));
	background.setFillColor(sf::Color::Color(0, 0, 0, 150));

	game->window->draw(background);
	PlayButton->render(game->window);

	ResCombo->render(game->window);
}

void UI::drawTowerUI(){
	game->window->draw(TowerSelectionBG);

	if (SelectedTower) {
		SellButton->update(game->MousePos);
		upgradeButton->update(game->MousePos);
		upgradeButton->setText("upgrade: " + std::to_string(SelectedTower->getUpgradeCost()) + "$");
		std::string LevelStr = "Level " + std::to_string(SelectedTower->getLevel());
		std::string DmgStr = "Damage: " + game->func->cutString(std::to_string(SelectedTower->getDmg()), 4);
		std::string ShootSpeedStr = "Shoot speed: " + game->func->cutString(std::to_string(SelectedTower->getShootTimer()), 4);
		if (SelectedTower->getLevel() >= SelectedTower->getMaxLevel()) {
			upgradeButton->setText("MAX");
			LevelStr += "(MAX)";
		}
		else {
			DmgStr += " -> " + game->func->cutString(std::to_string(SelectedTower->getDmg() * SelectedTower->getDmgMult()), 4);
			ShootSpeedStr += " -> " + game->func->cutString(std::to_string(SelectedTower->getShootTimer() -
				SelectedTower->getShootTimer() * SelectedTower->getBulletSpeedMult()), 4);
		}
		Text.setString(LevelStr);


		drawText(LevelStr, 15, { game->window->getSize().x / 2.f - 50, TowerSelectionBG.getPosition().y }, sf::Color::White);

		drawText(DmgStr, 15, {game->window->getSize().x / 2.f - 60, Text.getPosition().y + Text.getGlobalBounds().height * 1.3f }, sf::Color::Yellow);

		drawText(ShootSpeedStr, 15, { game->window->getSize().x / 2.f - 70, Text.getPosition().y + Text.getGlobalBounds().height * 1.3f }, sf::Color::Yellow);

		drawText("Bloons poped: " + std::to_string(SelectedTower->getBloonsPoped()), 15, { TowerSelectionBG.getPosition().x + 250.f,
			TowerSelectionBG.getPosition().y + TowerSelectionBG.getGlobalBounds().height * 0.4f }, sf::Color::Yellow);

		if (SellButton->isPressed()) {
			game->SellSound.play();
			game->money->Give(SelectedTower->getSellPrice());
			SelectedTower->setDeleted();
			SelectedTower = nullptr;
		}

		if (upgradeButton->isPressed() && game->money->get() - SelectedTower->getUpgradeCost() >= 0) {
			if (game->money->get() - SelectedTower->getUpgradeCost() >= 0) {
				game->money->Lose(SelectedTower->getUpgradeCost());
				SelectedTower->upgrade();
				SelectedTower = nullptr;
			}
		}

		upgradeButton->render(game->window);
		SellButton->render(game->window);
	}
	else {
		Text.setFillColor(sf::Color::Yellow);
		Text.setCharacterSize(20);
		for (size_t i = 0; i < TowersToSelect.size(); i++) {
			TowersToSelect[i]->render(game->window);
			Text.setString(std::to_string(Settings::Tower::Towers[i].TowerPrice) + "$");

			Text.setPosition(TowersToSelect[i]->getPos().x + TowersToSelect[i]->getSize().x / 2.f - Text.getGlobalBounds().width / 2.f,
				TowersToSelect[i]->getPos().y + TowersToSelect[i]->getSize().y - Text.getGlobalBounds().height / 2.f);

			game->window->draw(Text);
		}

		drawText("income: " + std::to_string(game->incomeMoney) + "$", 15,
			{ TowerSelectionBG.getPosition().x + TowerSelectionBG.getSize().x * 0.8f, TowerSelectionBG.getPosition().y + TowerSelectionBG.getSize().y * 0.2f },
			sf::Color::Yellow);
		drawText(std::to_string(static_cast<int>(game->incomeTimer.getTimer())), 20,
			{ Text.getPosition().x + Text.getGlobalBounds().width * 0.5f, Text.getPosition().y + Text.getGlobalBounds().height * 1.5f }, sf::Color::White);
	}
}

bool UI::isPrepPhaseOver(const float& dt){
	if (game->Bloons->getRoundEnd()) {
		if (PrepPhaseTimer.timerOver(dt)) {
			return true;
		}
	}

	return false;
}

const bool& UI::getPause() const{
	return Pause;
}
