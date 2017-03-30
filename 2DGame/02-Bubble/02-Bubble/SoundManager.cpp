#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {
	/*if (!levelmusic.openFromFile("sound/levelBGM.WAV")){
		std::cout << "error loading music" << std::endl;
	}
	if (!attackBuff.loadFromFile("sound/fortest.mp3")){
		std::cout << "error loading attack sound" << std::endl;
	}*/
}

void SoundManager::stopAll() {
	levelmusic.stop();
}

void SoundManager::playLevelMusic() {
	levelmusic.setLoop(true);
	levelmusic.setVolume(40);
	levelmusic.play();
}

void SoundManager::playAttack(){
	attack.setBuffer(attackBuff);
	attack.play();
}