#pragma once
#include "BloonManager.h"



class TileMap{
private:
	Func* func;

	struct BloonWayInfo {
		float distance;
		sf::Vector2i Dir;
	};
	sf::VertexArray vert;
	std::vector<std::vector<std::unique_ptr<Settings::Map::TileData>>> Tiles;
	sf::Texture Tileset;
	sf::Vector2f TileSize;
	std::vector<BloonWayInfo> BloonTravelWay;

	Settings::Map::TileData getTileByCharOfTiles(const std::vector<Settings::Map::TileData>& Tiles, const char& MapChar);
	void initTiles(const std::vector<std::string>& MapFromFile, const std::vector<Settings::Map::TileData>& TileDefs);
	void initBloonTravelWay();

	std::vector<sf::Vector2i> getBloonWayIndexes();

	sf::Vector2i getTileByChar(const char& MapChar);

	sf::Vector2i getStartTile();
	sf::Vector2i getEndTile();

public:

	TileMap(Func* func);

	void update();
	void render();

	const std::vector<BloonWayInfo>& getBloonTravelWay() const { return BloonTravelWay; };
    bool getCollisionTileByRect(const sf::FloatRect& Rect) const;
    sf::Vector2f getBloonStartPos();
	int danke;
};

