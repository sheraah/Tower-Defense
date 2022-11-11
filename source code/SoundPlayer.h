#pragma once
#include <SFML/Audio.hpp>
#include "Timer.h"


class SoundPlayer {
private:
	sf::SoundBuffer buff;
	std::vector<sf::Sound*> sound;
	int Volume;
	std::unique_ptr<Timer> timer;
	bool canPlay;


public:
	SoundPlayer(const std::string soundPath, const int& Volume, const float& PlayTimer) : Volume(Volume), 
		timer(std::unique_ptr<Timer>(new Timer(PlayTimer))), canPlay(true) {
		buff.loadFromFile(soundPath);
		sound.push_back(new sf::Sound(buff));
		sound[0]->setVolume(Volume);
	}

	~SoundPlayer() {
		for (size_t i = 0; i < sound.size(); i++) {
			delete sound[i];
			sound.erase(sound.begin() + i);
		}
	}

	void update(const float& dt) {
		if (canPlay) return;
		if (timer->timerOver(dt) && !canPlay) canPlay = true;
	}

	void play() {
	    if (!canPlay) return;

		bool Played = false;
		for (size_t i = 0; i < sound.size(); i++) {
			if (sound[i]->getStatus() == sf::Sound::Stopped && !Played) {
				sound[i]->play();
				Played = true;
				continue;
			}
			if (sound[i]->getStatus() == sf::Sound::Stopped) {
				delete sound[i];
				sound.erase(sound.begin() + i);
				continue;
			}
		}
		if (!Played) {
			sound.push_back(new sf::Sound(buff));
			size_t index = sound.size() - 1;
			sound[index]->setVolume(Volume);
			sound[index]->play();
		}

		canPlay = false;
		timer->reset();
	}
};