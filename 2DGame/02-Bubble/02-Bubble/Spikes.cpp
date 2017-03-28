#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Spikes.h"
#include "Game.h"
#include <time.h>
#include "Player.h"

enum SpikeAnims
{
	DOWN, COMING_UP, UP, COMING_DOWN
};



void Spikes::init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{
		
		spritesheet.loadFromFile("images/pues-spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(1.0/3.0, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(4);

		sprite->setAnimationSpeed(DOWN, 8);
		sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(COMING_UP, 8);
		sprite->addKeyframe(COMING_UP, glm::vec2((1.0 / 3.0), 0.f));
		sprite->addKeyframe(COMING_UP, glm::vec2((2.0 / 3.0), 0.f));

		sprite->setAnimationSpeed(UP, 8);
		sprite->addKeyframe(UP, glm::vec2((2.0 / 3.0), 0.f));

		sprite->setAnimationSpeed(COMING_DOWN, 8);
		sprite->addKeyframe(COMING_DOWN, glm::vec2((1.0 / 3.0), 0.f));
		sprite->addKeyframe(COMING_DOWN, glm::vec2(0.f, 0.f));
		

		sprite->changeAnimation(DOWN);	

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpikes.x), float(tileMapDispl.y + posSpikes.y)));

	//Player
	player = pl;
}



void Spikes::update(int deltaTime)
{

	if (!busy){//check if animation has to change
		glm::fvec2 posplayer = player->getPosPlayer();

		if (sprite->animation() == COMING_UP){
			sprite->changeAnimation(UP);
			busy = true;
			stamp = clock();
		}
		else if (sprite->animation() == COMING_DOWN){
			sprite->changeAnimation(DOWN);
			busy = true;
			stamp = clock();
		}

		if (posplayer.x + 64.0 > (posSpikes.x + 16) - 75 && posplayer.x + 64.0 < (posSpikes.x + 16) + 75){
			if (sprite->animation() == DOWN){
				sprite->changeAnimation(COMING_UP);
				busy = true;
				stamp = clock();
			}

		}
		else{
			if (sprite->animation() == UP){
				sprite->changeAnimation(COMING_DOWN);
				busy = true;
				stamp = clock();
			}
		}
		
	}
	else{//play current animation
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		glm::fvec2 posplayer = player->getPosPlayer();
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\n";

		if (sprite->animation() == UP){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == DOWN){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == COMING_UP){
			if (time >= 1.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == COMING_DOWN){
			if (time >= 1.9 / 8.0){
				busy = false;
			}
		}

	}
}

void Spikes::render()
{
	sprite->render();
}

void Spikes::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Spikes::getPosRender() {
	return posSpikes;
}

void Spikes::setPosition(const glm::vec2 &pos)
{
	posSpikes = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpikes.x), float(tileMapDispl.y + posSpikes.y)));
}

