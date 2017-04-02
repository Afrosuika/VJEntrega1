#include "SoundManager2.h"
#include <iostream>

SoundManager2::SoundManager2() {
	bigdooropening = false;
}

void SoundManager2::stopAll() {
	PlaySound(NULL, 0, 0);
}

void SoundManager2::BigDoorFinished(){
	bigdooropening = false;
}

void SoundManager2::playBigdoorOpen() {
	std::cout << (PlaySound(TEXT("sound/PoP_sounds/PoP_bigdoor_open.wav"), NULL, SND_ASYNC | SND_FILENAME))<<"\n";
	bigdooropening = true;
}

void SoundManager2::playEndLevel(){
		PlaySound(TEXT("sound/PoP_sounds/PoP_endlevel_fanfare.wav"), NULL, SND_FILENAME);
}

void SoundManager2::playEnemyHit() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_enemy_hit.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playGuillotineSnap() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_guillotine_snap.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playPrinceOomp() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_prince_oomp.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playPrinceSlash(){
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_prince_slash.wav"), NULL, SND_NOSTOP |SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playPrinceSteps() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_prince_steps.wav"), NULL, SND_NOSTOP | SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playPrinceUnsheathe() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_prince_unsheathe.wav"), NULL, SND_NOSTOP | SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playSpikesUp() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/PoP_sounds/PoP_spikes_up.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playWilhelm() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/other_sounds/wilhelm.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playShogunScream() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/other_sounds/shogun-eeeeyyaaaagh.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playPortalSound() {
	if (!bigdooropening){
		PlaySound(TEXT("sound/other_sounds/portal_sound.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SoundManager2::playMenuMusic() {
		PlaySound(TEXT("sound/other_sounds/arabian_night.wav"), NULL, SND_LOOP| SND_ASYNC | SND_FILENAME);
}

void SoundManager2::playCreditsMusic() {
	PlaySound(TEXT("sound/PoP_sounds/PoP_ending.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
}