#pragma once
#include "Tower.h"

class GameState;

class UI{

private:

	GameState* game;
	Timer PrepPhaseTimer;
	sf::Text Text;
	std::unique_ptr<Button> PlayButton;
	std::unique_ptr<Button> SellButton;
	std::unique_ptr<Button> upgradeButton;
	bool Pause;
	Tower* SelectedTower;
	sf::RectangleShape TowerSelectionBG;
	sf::RectangleShape HPFront;
	sf::RectangleShape HPBack;
	sf::Texture HPFrontTex;
	sf::Texture HPBackTex;
	std::vector<std::unique_ptr<ShapeWithHitbox>> TowersToSelect;
	std::unique_ptr<Combo<sf::Vector2u>> ResCombo;

	void initUI();
	void initPrepPhase();
	void initTowerUI(); 
	void initTowerInfoUI();
	void initText();
	void drawText(const std::string str, const unsigned int chSize, const sf::Vector2f Pos, const sf::Color color);

	void updateUI();
	void updatePausedScreen();
	void updateTowerUI();
	void updateText();

	void drawUI();
	void drawText();
	void drawPauseScreen();
	void drawTowerUI();
public:
	UI(GameState* game);

	void update();
	void render();
	void setSelectedTower(Tower* SelectedTower);
	Tower* getSelectedTower() { return SelectedTower; };
	const sf::FloatRect getTowerUIBounds() { return TowerSelectionBG.getGlobalBounds(); };

	bool isPrepPhaseOver(const float& dt);
	const bool& getPause() const;
	
};

