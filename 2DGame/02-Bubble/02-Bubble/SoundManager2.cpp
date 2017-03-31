#include "SoundManager2.h"
#include <iostream>

SoundManager2::SoundManager2() {
	/*if (!levelmusic.openFromFile("sound/levelBGM.WAV")){
		std::cout << "error loading music" << std::endl;
	}
	if (!attackBuff.loadFromFile("sound/fortest.mp3")){
		std::cout << "error loading attack sound" << std::endl;
	}*/
}

void SoundManager2::stopAll() {
	PlaySound(NULL, 0, 0);
}

void SoundManager2::playLevelMusic() {
	PlaySound(TEXT("sound/levelBGM.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

void SoundManager2::playAttack(){
	PlaySound(TEXT("sound/levelBGM.wav"), NULL, SND_ASYNC | SND_FILENAME);
}