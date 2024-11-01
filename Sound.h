#pragma once
#include <SFML/Audio.hpp>
class Sound
{
public:
	Sound();
	void playSound(const std::string& filename);
	void stopSound();
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
};

