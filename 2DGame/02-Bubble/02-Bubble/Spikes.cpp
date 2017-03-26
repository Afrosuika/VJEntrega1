#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Spikes.h"
#include "Game.h"
#include <time.h>

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

	posSpikes = position;
	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpikes.x), float(tileMapDispl.y + posSpikes.y)));

	//Player
	player = pl;
}



void Spikes::update(int deltaTime)
{
	//manage spikes behavior here
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

