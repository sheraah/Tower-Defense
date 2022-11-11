#pragma once
#include "TileMap.h"

class GameState{
private:
	sf::Event evnt;
	std::unique_ptr<sf::Music> music;
	sf::SoundBuffer PlaceSoundbuffer;
	sf::SoundBuffer SellSoundbuffer;
	sf::SoundBuffer LevelUpSoundbuffer;

	void pollEvents();

	void initVariables();
	void initMap();
	void initGui();
	void initObjects();

	void updateMouse();
	void updateBloons(const float& dt);
	void updateTowers(const float& dt);

	void renderTowers();

public:
	GameState(sf::RenderWindow* window);
	std::unique_ptr<Func> func;
	int Hp;
	int fps;
	const int MaxHp = 150;
	bool Dead; 
	bool MissionCompleted;
	sf::RenderWindow* window;
	std::unique_ptr<TileMap> Map;
	sf::Font font;
	sf::Vector2i MousePos;
	std::unique_ptr<BloonManager> Bloons;
	std::unique_ptr<UI> ui;
	std::vector<std::unique_ptr<Tower>> Towers;
	std::unique_ptr<Money> money;
	sf::Sound PlaceSound;
	sf::Sound SellSound;
	sf::Sound LevelUpSound;
	Timer incomeTimer;
	int incomeMoney;
	void update(const float& dt);
	void draw();
};

