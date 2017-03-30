#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Portagran.h"
#include "Game.h"
#include <time.h>
#include "Player.h"

enum PortagranAnims
{
	CLOSED, OPENING, OPEN
};



void Portagran::init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{
	busy = false;
	mustopen = false;
	oberta = false;
	spritesheet.loadFromFile("images/portagran-spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(96, 128), glm::vec2(1.0 / 6.0, 1.0 / 3.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPENING, 8);
	sprite->addKeyframe(OPENING, glm::vec2((1.0 / 6.0), 0.f));
	sprite->addKeyframe(OPENING, glm::vec2((2.0 / 6.0), 0.f));
	sprite->addKeyframe(OPENING, glm::vec2((3.0 / 6.0), 0.f));
	sprite->addKeyframe(OPENING, glm::vec2((4.0 / 6.0), 0.f));
	sprite->addKeyframe(OPENING, glm::vec2((5.0 / 6.0), 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((1.0 / 6.0), (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((2.0 / 6.0), (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((3.0 / 6.0), (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((4.0 / 6.0), (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((5.0 / 6.0), (1.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, (2.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((1.0 / 6.0), (2.0 / 3.0)));
	sprite->addKeyframe(OPENING, glm::vec2((2.0 / 6.0), (2.0 / 3.0)));


	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2((3.0 / 6.0), (2.0/3.0)));


	sprite->changeAnimation(CLOSED);

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortagran.x), float(tileMapDispl.y + posPortagran.y)));

	//Player
	player = pl;
}



void Portagran::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!busy){//check if animation has to change
		glm::fvec2 posplayer = player->getPosPlayer();

		if (sprite->animation() == CLOSED){
			if (mustopen){
				sprite->changeAnimation(OPENING);
				busy = true;
				stamp = clock();
			}
			else{
				sprite->changeAnimation(CLOSED);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == OPENING){
			sprite->changeAnimation(OPEN);
			oberta = true;
			busy = true;
			stamp = clock();
		}

	}
	else{//play current animation
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		glm::fvec2 posplayer = player->getPosPlayer();
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\n";

		if (sprite->animation() == CLOSED){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == OPEN){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == OPENING){
			if (time >= 13.9 / 8.0){
				busy = false;
			}
		}

	}
}

void Portagran::render()
{
	sprite->render();
}

void Portagran::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Portagran::getPosRender() {
	return posPortagran;
}

void Portagran::setPosition(const glm::vec2 &pos)
{
	posPortagran = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPortagran.x), float(tileMapDispl.y + posPortagran.y)));
}

void Portagran::setMustopen(bool cond)
{
	mustopen = cond;
}

bool Portagran::getOberta(){
	return oberta;
}