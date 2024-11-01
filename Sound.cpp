#include "Sound.h"
#include <iostream>

Sound::Sound()
{
}

void Sound::playSound(const std::string& filename)
{
	if (!buffer.loadFromFile("assets/sound/" + filename + ".wav"))
	{
		std::cout << "Error loading sound file: " << filename << std::endl;
	}
	sound.setBuffer(buffer);
	sound.play();
	std::cout << "Playing sound: " << filename << std::endl;
}

void Sound::stopSound()
{
	sound.stop();
}
