#include "Game.h"

int main() {
    srand(unsigned(time(0)));
    Game game({ 1000,800 }, "Tower Defense");

    game.run();
}