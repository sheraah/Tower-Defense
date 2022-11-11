#pragma once
#include <fstream>
#include <assert.h>
#include <sstream>
#include <SFML/Graphics.hpp>

#define PI 3.14159265359

class Func {
private:
	sf::RenderWindow* window;
	sf::Vector2u FirstRes;

public:
	Func(sf::RenderWindow* window): window(window) { FirstRes = window->getSize(); };

	const float toX() {
		return window->getSize().x / FirstRes.x;
	}

	const float toY() {
		return window->getSize().y / FirstRes.y;
	}

	const sf::Vector2u& getFirstRes() const { return FirstRes; };

	std::vector<std::string> ReadFromFile(const std::string& FilePath) {
		std::vector<std::string> result;

		std::ifstream in(FilePath);
		if (!in.is_open()) throw "No file found";

		std::string line;
		while (std::getline(in, line)) {
			result.push_back(line);
		}

		in.close();
		return result;
	}

	bool isCharNumber(const char& ch) {
		return ch >= '0' && ch <= '9';
	}


	template<class T>
	T StrToNum(const std::string Str) {
		T Value = 0;

		std::stringstream ss;
		ss << Str;
		ss >> Value;

		return Value;
	}

	const std::string cutString(const std::string str, const size_t& index) {
		std::string result = "";

		for (size_t i = 0; i < index; i++) {
			result += str[i];
		}

		return result;
	}


	float RadToDeg(float Rad) {
		return (180.f * Rad / PI);
	}

	float DegToRad(float Deg) {
		return Deg * PI / 180.f;
	}

	float CalcTanAngle(float GegenKathete, float AnKathete) {
		return RadToDeg(atan(GegenKathete / AnKathete));
	}

     float CalcRotationAngle(const sf::Vector2f From, const sf::Vector2f To, const float CurrRotaion) {
		return RadToDeg(atan2(To.y - From.y, To.x - From.x)) - CurrRotaion + 90.f;
	}

	float getDistOf2dCoords(const sf::Vector2f P1, const sf::Vector2f P2) {
		return sqrt(pow(P2.x - P1.x, 2) + pow(P2.y - P1.y, 2));
	}

	sf::Vector2f NormVector(const sf::Vector2f Origin, const sf::Vector2f vec) {
		sf::Vector2f Dir = vec - Origin;
		float Length = sqrt(pow(Dir.x, 2) + pow(Dir.y, 2));
		return sf::Vector2f(Dir.x / Length, Dir.y / Length);
	}

	sf::Vector2f getPointOnCircle(const sf::Vector2f CircleCenter, const float radius, const float angle) {
		return { CircleCenter.x + radius * cos(DegToRad(angle - 90.f)), CircleCenter.y + radius * sin(DegToRad(angle - 90.f)) };
	}

	bool isShapeInCircle(const sf::FloatRect Bounds, const sf::Vector2f& CircleCenter, const float Radius) {
		sf::Vector2f TL = { Bounds.left, Bounds.top };
		sf::Vector2f TR = { TL.x + Bounds.width, TL.y };
		sf::Vector2f BL = { TL.x, TL.y + Bounds.height };
		sf::Vector2f BR = { TR.x, BL.y };

		float PowRadius = pow(Radius, 2);

		if (pow(TL.x - CircleCenter.x, 2) + pow(TL.y - CircleCenter.y, 2) <= PowRadius) return true;
		if (pow(TR.x - CircleCenter.x, 2) + pow(TR.y - CircleCenter.y, 2) <= PowRadius) return true;
		if (pow(BL.x - CircleCenter.x, 2) + pow(BL.y - CircleCenter.y, 2) <= PowRadius) return true;
		if (pow(BR.x - CircleCenter.x, 2) + pow(BR.y - CircleCenter.y, 2) <= PowRadius) return true;

		return false;
	}

	sf::RenderWindow* getWindow() { return window; }
};