#ifndef _SOUND_MANAGER2_INCLUDE
#define _SOUND_MANAGER2_INCLUDE

#include <iostream>
#include <Windows.h>

class SoundManager2
{

public:
	SoundManager2();
	void BigDoorFinished();
	void stopAll();
	void playBigdoorOpen();
	void playEndLevel();
	void playEnemyHit();
	void playGuillotineSnap();
	void playPrinceOomp();
	void playPrinceSlash();
	void playPrinceSteps();
	void playPrinceUnsheathe();
	void playSpikesUp();
	void playWilhelm();
	void playShogunScream();
	void playPortalSound();
	void playMenuMusic();
	void playCreditsMusic();

private:
	bool bigdooropening;

};

#endif // _SOUND_MANAGER2_INCLUDE