#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Portal.h"
#include "Game.h"
#include <time.h>
#include "Player.h"

enum PortalAnims
{
	READY, CHARGING
};



void Portal::init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{
	busy = false;
	spritesheet.loadFromFile("images/portal-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(CHARGING, 8);
	sprite->addKeyframe(CHARGING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(READY, 8);
	sprite->addKeyframe(READY, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(READY, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(READY, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(READY, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(READY, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(READY, glm::vec2(0.5f, 0.f));


	sprite->changeAnimation(READY);

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));

	//Player
	player = pl;
}



void Portal::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!busy){//check if animation has to change
		glm::fvec2 posplayer = player->getPosPlayer();

		if (sprite->animation() == CHARGING){
			sprite->changeAnimation(READY);
		}

		else if (sprite->animation() == READY){
			if (posplayer.x + 64.0 > (posPortal.x + 36) - 18 && posplayer.x + 64.0 < (posPortal.x + 36) + 18){
				if (((posplayer.y + 8) - posPortal.y) > -10 && ((posplayer.y - 8) - posPortal.y) < 10){
					if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
						glm::fvec2 pospair = pair->getPosRender();
						player->setPosition(glm::vec2((pospair.x +36)-64,pospair.y - 8));
						manager->playPortalSound();
						pair->pairIsUsed();
						sprite->changeAnimation(CHARGING);
						busy = true;
						stamp = clock();
					}
				}
			}
		}
	}

	else{//play current animation
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		glm::fvec2 posplayer = player->getPosPlayer();
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\n";

		if (sprite->animation() == CHARGING){
			if (time >= 16.0 / 8.0){
				busy = false;
			}
		}

		else{			
				busy = false;			
		}
	}

}

void Portal::render()
{
	sprite->render();
}

void Portal::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Portal::getPosRender() {
	return posPortal;
}

void Portal::setPosition(const glm::vec2 &pos)
{
	posPortal = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortal.x), float(tileMapDispl.y + posPortal.y)));
}

void Portal::setSoundManager(SoundManager2* man){
	manager = man;
}

void Portal::setPair(Portal* parella){
	pair = parella;
}

void Portal::pairIsUsed(){
	sprite->changeAnimation(CHARGING);
	busy = true;
	stamp = clock();
}

