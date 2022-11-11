#include "TileMap.h"

TileMap::TileMap(Func* func) :func(func) {
	Tileset.loadFromFile(Settings::Map::TilesetPath);
	sf::Vector2f TileSizeInImage;
	TileSizeInImage.x = Tileset.getSize().x / Settings::Map::TilesetLayers.x;
	TileSizeInImage.y = Tileset.getSize().y / Settings::Map::TilesetLayers.y;
	initTiles(func->ReadFromFile(Settings::Map::MapFilePath), Settings::Map::TileDefs);

	initBloonTravelWay();

	vert.setPrimitiveType(sf::Quads);

	vert.resize(Tiles.size() * Tiles[0].size() * 4);

	for (size_t i = 0; i < Tiles.size(); i++) {
		for (size_t j = 0; j < Tiles[i].size(); j++) {
			for (size_t k = 0; k < Tiles[i][j]->LayersToDraw.size(); k++) {
				sf::Vector2f Pos = { j * TileSize.x, i * TileSize.y };
				sf::Vector2f TexPos = { Tiles[i][j]->LayersToDraw[k].x * TileSizeInImage.x,  Tiles[i][j]->LayersToDraw[k].y * TileSizeInImage.y };

				vert.append(sf::Vertex(sf::Vector2f(Pos), sf::Vector2f(TexPos)));
				vert.append(sf::Vertex(sf::Vector2f({ Pos.x + TileSize.x, Pos.y }), sf::Vector2f({ TexPos.x + TileSizeInImage.x, TexPos.y })));
				vert.append(sf::Vertex(sf::Vector2f({ Pos.x + TileSize.x, Pos.y + TileSize.y }), sf::Vector2f({ TexPos.x + TileSizeInImage.x, TexPos.y + TileSizeInImage.y })));
				vert.append(sf::Vertex(sf::Vector2f({ Pos.x,  Pos.y + TileSize.y }), sf::Vector2f({ TexPos.x, TexPos.y + TileSizeInImage.y })));
			}
		}
	}
}

void TileMap::initTiles(const std::vector<std::string>& MapFromFile, const std::vector<Settings::Map::TileData>& TileDefs){
	for (size_t i = 0; i < MapFromFile.size(); i++) {
		std::vector<std::unique_ptr<Settings::Map::TileData>> Row;
		for (size_t j = 0; j < MapFromFile[i].length(); j++) {
	    	if (MapFromFile[i][j] == ' ') continue;
			Settings::Map::TileData Tile =  getTileByCharOfTiles(TileDefs, MapFromFile[i][j]);
			Row.emplace_back(std::unique_ptr<Settings::Map::TileData>(new Settings::Map::TileData({ Tile.MapChar, Tile.Collision, Tile.LayersToDraw })));
		}
	    Tiles.emplace_back(std::move(Row));
	}

	TileSize.x = static_cast<float>(func->getWindow()->getSize().x) / Tiles[0].size();
	TileSize.y = static_cast<float>(func->getWindow()->getSize().y) / Tiles.size();
}

void TileMap::initBloonTravelWay(){
	std::vector<sf::Vector2i> BloonWay = getBloonWayIndexes();

	sf::Vector2i CurrentDir = { BloonWay[1].x - BloonWay[0].x, BloonWay[1].y - BloonWay[0].y };
	int count = 6;

	for (size_t i = 2; i < BloonWay.size(); i++) {

		sf::Vector2i Dir = { BloonWay[i].x - BloonWay[i - 1].x, BloonWay[i].y - BloonWay[i - 1].y };

		if (Dir != CurrentDir || i == BloonWay.size() - 1) {
			if (i == BloonWay.size() - 1) count += 5;
			
			float Dist = 0;
			if (CurrentDir.x == 0) Dist = count * TileSize.y;
			else Dist = count * TileSize.x;

			BloonTravelWay.push_back({ Dist, CurrentDir });

			CurrentDir = Dir;
			count = 1;
		}
		else {
			count++;
		}
	}
}

std::vector<sf::Vector2i> TileMap::getBloonWayIndexes()
{

	auto getNearestTile = [this](const sf::Vector2i& TileIndex) {
		std::vector<sf::Vector2i> result;

		if (TileIndex.x + 1 < Tiles.size()) {
			if (Tiles[TileIndex.x + 1][TileIndex.y]->MapChar == 's' || Tiles[TileIndex.x + 1][TileIndex.y]->MapChar == 'E') {
				result.push_back(sf::Vector2i(TileIndex.x + 1, TileIndex.y));
			}
		}
		if (TileIndex.y + 1 < Tiles[0].size()) {
			if (Tiles[TileIndex.x][TileIndex.y + 1]->MapChar == 's' || Tiles[TileIndex.x][TileIndex.y + 1]->MapChar == 'E') {
				result.push_back(sf::Vector2i(TileIndex.x, TileIndex.y + 1));
			}
		}
		if (TileIndex.x - 1 >= 0) {
			if (Tiles[TileIndex.x - 1][TileIndex.y]->MapChar == 's' || Tiles[TileIndex.x - 1][TileIndex.y]->MapChar == 'E') {
				result.push_back(sf::Vector2i(TileIndex.x - 1, TileIndex.y));
			}
		}
		if (TileIndex.y - 1 >= 0) {
			if (Tiles[TileIndex.x][TileIndex.y - 1]->MapChar == 's' || Tiles[TileIndex.x][TileIndex.y - 1]->MapChar == 'E') {
				result.push_back(sf::Vector2i(TileIndex.x, TileIndex.y - 1));
			}
		}
		return result;
	};

	auto End = getEndTile();
	auto PrevTile = sf::Vector2i{ 0, 0 };
	auto CurrentTile = getStartTile();

	std::vector<sf::Vector2i> BloonWay = { { CurrentTile.y, CurrentTile.x} };

	while (true) {

		for (const auto& C : getNearestTile({ CurrentTile.x, CurrentTile.y })) {
			if (C != PrevTile) {
				PrevTile = CurrentTile;
				CurrentTile = C;
				break;
			}
		}

		BloonWay.push_back({ CurrentTile.y, CurrentTile.x });

		if (CurrentTile.x == End.x && CurrentTile.y == End.y) break;
	}

	return BloonWay;
}

sf::Vector2i TileMap::getTileByChar(const char& MapChar)
{	
	for (int i = 0; i < Tiles.size(); i++) {
		for (int j = 0; j < Tiles[i].size(); j++) {
			if (Tiles[i][j]->MapChar == MapChar) {
				return sf::Vector2i{ i , j };
			}
		}
	}
}

sf::Vector2i TileMap::getStartTile(){
	return getTileByChar('S');
}

sf::Vector2i TileMap::getEndTile(){
	return getTileByChar('E');
}

bool TileMap::getCollisionTileByRect(const sf::FloatRect& Rect) const{
	std::vector<bool> allCollisions;

	auto checkX = [this](sf::Vector2i& vec) {
		if (vec.x < 0) vec.x = 0; if (vec.x >= Tiles[0].size()) vec.x = Tiles[0].size() - 1;
	};

	auto checkY = [this](sf::Vector2i& vec) {
		if (vec.y < 0) vec.y = 0; if (vec.y >= Tiles.size()) vec.y = Tiles.size() - 1;
	};

	sf::Vector2i TL = { static_cast<int>(Rect.left / TileSize.x), static_cast<int>(Rect.top / TileSize.y) }; checkX(TL); checkY(TL);
	sf::Vector2i TR = { static_cast<int>((Rect.left + Rect.width) / TileSize.x), TL.y }; checkX(TR);
	sf::Vector2i BL = { TL.x, static_cast<int>((Rect.top + Rect.height) / TileSize.y) }; checkY(BL);
	sf::Vector2i BR = { TR.x , BL.y };
	sf::Vector2i Mid = { static_cast<int>((Rect.left + Rect.width / 2.f) / TileSize.x), 
		static_cast<int>((Rect.top + Rect.height / 2.f) / TileSize.y) };
	checkX(Mid);
	checkY(Mid);

	allCollisions.push_back(Tiles[TL.y][TL.x]->Collision);
	allCollisions.push_back(Tiles[TR.y][TR.x]->Collision);
	allCollisions.push_back(Tiles[BL.y][BL.x]->Collision);
	allCollisions.push_back(Tiles[BR.y][BR.x]->Collision);
	allCollisions.push_back(Tiles[Mid.y][Mid.x]->Collision);


	 for (const bool& A : allCollisions) if (!A) return false;

    return true;
}

Settings::Map::TileData TileMap::getTileByCharOfTiles(const std::vector<Settings::Map::TileData>& Tiles, const char& MapChar) {
	for (const auto& T : Tiles) {
		if (T.MapChar == MapChar) return T;
	}

	return { 0,0,{{0,0}} };
}


void TileMap::update(){

}

void TileMap::render(){
	func->getWindow()->draw(vert, &Tileset);
}

sf::Vector2f TileMap::getBloonStartPos(){
	sf::Vector2i index = getStartTile();

	return sf::Vector2f({ (index.y * TileSize.x + TileSize.x / 2) - BloonTravelWay[0].Dir.x * TileSize.x * 5.f,
		(index.x * TileSize.y + TileSize.y / 2) - BloonTravelWay[0].Dir.y * TileSize.y * 5.f });
}
