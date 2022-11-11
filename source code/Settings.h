#pragma once
#include <SFML/Graphics.hpp>

namespace Settings {
	namespace Game {
		const int StartMoney = 650;
		const float IncomeTimer = 15.f;
		const int IncomeMoneyStart = 350;
		const std::string GameMusicSoundPath = "Sounds/MenuMusic.Wav";
		const int GameMusicVolume = 5;
		const std::string PlaceSoundPath = "Sounds/PlaceSound.wav";
		const int PlaceSoundVolume = 100;
		const std::string SellSoundPath = "Sounds/SellSound.wav";
		const int SellSoundVolume = 100;
		const std::string LevelUpSoundPath = "Sounds/LevelUpSound.wav";
		const int LevelUpSoundVolume = 5;
		const std::string PopSoundPath = "Sounds/BloonPopSound.wav";
		const int PopSoundVolume = 10;
		const float PrepPhaseTimer = 10.f;
	}

	namespace Map {
		const std::string MapFilePath = "txt/Level1/Level.txt";
		const std::string TilesetPath = "Textures/Tilesets/Tileset.png";
		const sf::Vector2u TilesetLayers = { 8, 2 };

		struct TileData {
			char MapChar;
			bool Collision;
			std::vector<sf::Vector2i> LayersToDraw;
		};

		const std::vector<TileData> TileDefs = {
			{ '1', true, {{ 2, 0 }} },
			{ '2', true, {{ 3, 0 }} },
			{ '4', false, {{ 2, 0 }, { 4, 1 }} },
			{ '5', false, {{ 2, 0 }, { 5, 1 }} },
			{ '6', false, {{ 2, 0 }, { 6, 1 }} },
			{ '7', false, {{ 2, 0 }, { 7, 1 }} },
			{ '0', true, {{ 5, 0 }} },
			{ 'S', false, {{ 0, 1 }} },
			{ 's', false, {{ 0, 1 }} },
			{ 'E', false, {{ 0, 1 }} },
			{ '8', false, {{ 1, 1 }} }
		};
	}

	namespace Bloon {
		struct BloonInfo {
		public:
			std::string Name;
			float Speed;
			float Hp;
			int Dmg;
			std::string TexturePath;
			sf::Vector2f BloonSize;
		};


		const std::vector<BloonInfo> Bloons = {
			{"Red", 40.f, 1.f, 1, "Textures/Bloons/Red.png", {40.f, 40.f}},
			{"Blue", 50.f, 2.f, 3, "Textures/Bloons/Blue.png", {45.f, 45.f}},
			{"Green", 55.f, 2.f, 5, "Textures/Bloons/Green.png", {48.f, 48.f}},
			{"Yellow", 65.f, 3.f, 7, "Textures/Bloons/Yellow.png", {50.f, 50.f}},
			{"Pink", 70.f, 4.f, 9, "Textures/Bloons/Pink.png", {40.f, 40.f}},
			{"Black", 80.f, 4.f, 11, "Textures/Bloons/Black.png", {45.f, 45.f}},
			{"Purple", 85.f, 6.f, 18, "Textures/Bloons/Purple.png", {35.f, 35.f}},
			{"Lead", 55.f, 15.f, 35, "Textures/Bloons/Lead.png", {60.f, 60.f}}
		};
	}

	namespace Tower {
		struct TowerInfo {
			std::string TexturePath;
			sf::Vector2f TowerSize;
			int TowerPrice;
			float AttackRadius;
			float MaxBloonHit;
			std::vector<int> upgradeCosts;
			sf::Vector2f BulletSize;
			float BulletSpeed;
			float ShootTimerInSec;
			std::vector<float> ShootingAngles;
			float Dmg;

			float DmgMultPerLevel;
			float ShootingSpeedMultPerLevel;
			float RadiusMultPerLevel;
			float MaxBloonHitPerLevel;
		};


		const std::vector<TowerInfo> Towers = {
			{"Textures/Towers/BulletTank", {45.f, 45.f}, 450, 50.f, 1.f, {400, 1000, 1650, 2675}, {10.f, 10.f}, 500.f, 1.4f, {0.f}, 1.f, 1.2f, 0.05f, 1.2f, 1.f },
			{"Textures/Towers/LaserTank", {40.f, 40.f}, 750, 40.f, 1.5f, {900, 1650, 2675, 4000}, {10.f, 15.f}, 700.f, 0.9f, {-90, 0.f, 90.f}, 1.f, 1.2f, 0.07f, 1.3f, 0.8f },
			{"Textures/Towers/HeavyTank", {60.f, 60.f}, 2000, 70.f, 4.f, {2100, 4775}, {18.f, 30.f}, 300.f, 1.5f, {-85, -5, 5, 85.f}, 3.5f, 1.3f, 0.1f, 1.4f, 2.5f }
		};
	}
}