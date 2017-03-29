#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Guillotina.h"
#include "Game.h"
#include <time.h>
#include "Player.h"
#include "Soldier.h"

enum GuillotinaAnims
{
	OPEN, MOVING
};



void Guillotina::init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{

	damagedone = false;
	busy = false;
	spritesheet.loadFromFile("images/guillotina-spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MOVING, 8);
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.75, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuillotina.x), float(tileMapDispl.y + posGuillotina.y)));

	//Player
	player = pl;
}



void Guillotina::update(int deltaTime)
{

	if (!busy){//check if animation has to change
		glm::fvec2 posplayer = player->getPosPlayer();

		if (posplayer.x + 64.0 > (posGuillotina.x + 32) - 150 && posplayer.x + 64.0 < (posGuillotina.x + 32) + 150){
			//el jugador està a prop de la trampa, i aquesta s'ha de moure
			//cout << "detecta el jugador a prop\njugador= " << posplayer.x << "guillotina= " << posGuillotina.x << "\n";
				sprite->changeAnimation(MOVING);
				damagedone = false;
				busy = true;
				stamp = clock();
		}
		else{
			//el jugador està lluny de la trampa i aquesta s'ha d'aturar
			//cout << "no detecta el jugador a prop\njugador= " << posplayer.x << "guillotina= " << posGuillotina.x << "\n";
			sprite->changeAnimation(OPEN);
			busy = true;
			stamp = clock();
		}
	}
	else{//play current animation
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		glm::fvec2 posplayer = player->getPosPlayer();
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\n";

		if (sprite->animation() == OPEN){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == MOVING){
			if (time >= 12.0 / 8.0 && time <= 13.0 / 8.0){
				if (!damagedone){
					cout << "la guillotina ataca!\n";
					if (posplayer.x + 64.0 > (posGuillotina.x + 12) - 15 && posplayer.x + 64.0 < (posGuillotina.x + 12) + 15){
						if (((posplayer.y + 8) - posGuillotina.y) > -10 && ((posplayer.y - 8) - posGuillotina.y) < 10){
							//el jugador està massa a prop de la trampa i aquesta el mata
							player->getSliced();
						}
					}
					for (Soldier* soldat : soldats){
						glm::fvec2 possoldat = soldat->getPosRender();
						if (possoldat.x + 64.0 > (posGuillotina.x + 42) - 15 && possoldat.x + 64.0 < (posGuillotina.x + 42) + 15){
							if (((possoldat.y + 8) - posGuillotina.y) > -10 && ((possoldat.y - 8) - posGuillotina.y) < 10){
								//el soldat està massa a prop de la trampa i aquesta el mata
								soldat->getSliced();
							}
						}
					}
					damagedone = true;
				}

			}
			if (time >= 15.9 / 8.0){
				busy = false;
			}
		}

	}
}

void Guillotina::render()
{
	sprite->render();
}

void Guillotina::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Guillotina::getPosRender() {
	return posGuillotina;
}

void Guillotina::setPosition(const glm::vec2 &pos)
{
	posGuillotina = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuillotina.x), float(tileMapDispl.y + posGuillotina.y)));
}

void Guillotina::setSoldiers(vector<Soldier *> &vec)
{
	soldats = vec;
}
