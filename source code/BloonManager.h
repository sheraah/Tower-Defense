#pragma once
#include "Bloon.h"

class GameState;
class BloonManager{
private:
	std::vector<std::unique_ptr<ShapeWithHitbox>> BloonShapes;

	struct BloonSpawnInfo {
		const int BloonType;
		float Timer;
	};

	std::vector<std::unique_ptr<Bloon>> Bloons;
	std::vector<BloonSpawnInfo> BloonSpawns;
	GameState* game;
	std::unique_ptr<SoundPlayer> PopSound;

	Timer BloonSpawnTimer;
	size_t BloonSpawnerIndex;
	bool roundEnd;
	int CurrentRound;

	void initVariables();
	void initBloonSpawns();
	void initBloons();
	
	void updateBloonSpawns(const float& dt);
public:
	BloonManager(GameState* map);

	void initNextRound();
	void update(const float& dt);
	bool updateBloons(const float& dt, const size_t& i);
	void render();

	const std::vector<std::unique_ptr<Bloon>>& getBloons()const { return Bloons; };
	bool getRoundEnd()  { return roundEnd && Bloons.size() <= 0; };
	const int& getRound() const { return CurrentRound; };
};



