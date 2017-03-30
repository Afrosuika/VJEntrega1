#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <iostream>
#include <SFML/Audio.hpp>

class SoundManager
{

public:
	SoundManager();
	void stopAll();
	void playLevelMusic();
	void playAttack();

private:
	sf::Music levelmusic;
	sf::SoundBuffer attackBuff;
	sf::Sound attack;

};

#endif // _SOUND_MANAGER_INCLUDE