#pragma once
#include "GameState.h"

class Game {
private:
	std::unique_ptr<sf::RenderWindow> window;
	sf::Clock clock;
	float deltaTime;

	std::unique_ptr<GameState> game;

	void init(const sf::VideoMode& WindowSize, const sf::String& AppName);
	void render();

public:

	Game(const sf::VideoMode& WindowSize, const sf::String& AppName);

	void run();
};