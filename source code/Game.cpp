#include "Game.h"

Game::Game(const sf::VideoMode& WindowSize, const sf::String& AppName){
	init(WindowSize, AppName);
}

void Game::init(const sf::VideoMode& WindowSize, const sf::String& AppName){
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(WindowSize,  AppName,  sf::Style::Close));

	game = std::unique_ptr<GameState>(new GameState(window.get()));
}


void Game::render() {
	window->clear();

	game->draw();

	window->display();
}

void Game::run(){
	while (window->isOpen()) {
		deltaTime = clock.restart().asSeconds();

        game->update(deltaTime);

		render();		
	}
}
