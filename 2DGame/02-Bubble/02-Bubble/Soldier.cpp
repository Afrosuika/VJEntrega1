#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Soldier.h"
#include "Game.h"
#include <time.h>
#include "Player.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum SoldierAnims
{
	RIGHT_STAND, LEFT_STAND, RIGHT_STEP_FORWARD, LEFT_STEP_FORWARD, RIGHT_STEP_BACK, LEFT_STEP_BACK, RIGHT_ATTACK, LEFT_ATTACK,
	RIGHT_FLINCH, LEFT_FLINCH, RIGHT_DIE, LEFT_DIE, RIGHT_DEAD, LEFT_DEAD, RIGHT_SPIKEDEATH, LEFT_SPIKEDEATH
};

enum SoldierStates
{
	IDLE, FIGHTING
};




void Soldier::init(Player *pl, int t, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{
	srand(time(NULL));
	type = t;
	alive = true;
	state = IDLE;
	takingdamage = false;
	aggrodistance = 150;
	dealtdamage = false;



	if (type == 0) {
		hp = 4;
		spritesheet.setWrapS(GL_MIRRORED_REPEAT);
		spritesheet.loadFromFile("images/soldier-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);

		sprite = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.2, 0.05), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(16);

		sprite->setAnimationSpeed(RIGHT_ATTACK, 8);
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.6f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.8f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.f, 0.05f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.8f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2f, 0.f));

		sprite->setAnimationSpeed(LEFT_ATTACK, 8);
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.8f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-1.0f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2f, 0.05f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-1.0f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4f, 0.f));
	}
	else {
		hp = 8;
		spritesheet.setWrapS(GL_MIRRORED_REPEAT);
		spritesheet.loadFromFile("images/soldier-sprite-alt.png", TEXTURE_PIXEL_FORMAT_RGBA);

		sprite = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.2, 0.05), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(16);

		sprite->setAnimationSpeed(RIGHT_ATTACK, 8);
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.6f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.8f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.f, 0.05f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.8f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4f, 0.f));
		sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2f, 0.f));

		sprite->setAnimationSpeed(LEFT_ATTACK, 8);
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.8f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-1.0f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2f, 0.05f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-1.0f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6f, 0.f));
		sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4f, 0.f));
	}


	sprite->setAnimationSpeed(RIGHT_STAND, 8);
	sprite->addKeyframe(RIGHT_STAND, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(LEFT_STAND, 8);
	sprite->addKeyframe(LEFT_STAND, glm::vec2(-0.2f, 0.f));

	sprite->setAnimationSpeed(RIGHT_STEP_FORWARD, 8);
	sprite->addKeyframe(RIGHT_STEP_FORWARD, glm::vec2(0.2f, 0.05f));

	sprite->setAnimationSpeed(LEFT_STEP_FORWARD, 8);
	sprite->addKeyframe(LEFT_STEP_FORWARD, glm::vec2(-0.4f, 0.05f));

	sprite->setAnimationSpeed(RIGHT_STEP_BACK, 8);
	sprite->addKeyframe(RIGHT_STEP_BACK, glm::vec2(0.2f, 0.05f));

	sprite->setAnimationSpeed(LEFT_STEP_BACK, 8);
	sprite->addKeyframe(LEFT_STEP_BACK, glm::vec2(-0.4f, 0.05f));

	sprite->setAnimationSpeed(RIGHT_FLINCH, 8);
	sprite->addKeyframe(RIGHT_FLINCH, glm::vec2(0.4f, 0.05f));
	sprite->addKeyframe(RIGHT_FLINCH, glm::vec2(0.4f, 0.05f));
	sprite->addKeyframe(RIGHT_FLINCH, glm::vec2(0.4f, 0.05f));

	sprite->setAnimationSpeed(LEFT_FLINCH, 8);
	sprite->addKeyframe(LEFT_FLINCH, glm::vec2(-0.6f, 0.05f));
	sprite->addKeyframe(LEFT_FLINCH, glm::vec2(-0.6f, 0.05f));
	sprite->addKeyframe(LEFT_FLINCH, glm::vec2(-0.6f, 0.05f));

	sprite->setAnimationSpeed(RIGHT_DIE, 8);
	sprite->addKeyframe(RIGHT_DIE, glm::vec2(0.6f, 0.05f));
	sprite->addKeyframe(RIGHT_DIE, glm::vec2(0.8f, 0.05f));
	sprite->addKeyframe(RIGHT_DIE, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(RIGHT_DIE, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(LEFT_DIE, 8);
	sprite->addKeyframe(LEFT_DIE, glm::vec2(-0.8f, 0.05f));
	sprite->addKeyframe(LEFT_DIE, glm::vec2(-1.0f, 0.05f));
	sprite->addKeyframe(LEFT_DIE, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(LEFT_DIE, glm::vec2(-0.4f, 0.1f));

	sprite->setAnimationSpeed(RIGHT_DEAD, 8);
	sprite->addKeyframe(RIGHT_DEAD, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(LEFT_DEAD, 8);
	sprite->addKeyframe(LEFT_DEAD, glm::vec2(-0.4f, 0.1f));

	sprite->setAnimationSpeed(RIGHT_SPIKEDEATH, 8);
	sprite->addKeyframe(RIGHT_SPIKEDEATH, glm::vec2(0.4f, 0.1f));

	sprite->setAnimationSpeed(LEFT_SPIKEDEATH, 8);
	sprite->addKeyframe(LEFT_SPIKEDEATH, glm::vec2(-0.6f, 0.1f));





	sprite->changeAnimation(RIGHT_STAND);



	posSoldier = position;
	sprite->setPosition(glm::vec2(posSoldier.x, posSoldier.y));

	//Player
	player = pl;
}



void Soldier::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!busy){//current animation finished, see which triggers next
		glm::fvec2 posplayer = player->getPosPlayer();
		spikedanger = false;//primer comprobem si està a sobre d'unes punxes
		for (Spikes* theSpike : spikeTraps){
			glm::ivec2 spikepos = theSpike->getPosRender();
			if (posSoldier.x + 34.0 > (spikepos.x + 16) - 10 && posSoldier.x + 34.0 < (spikepos.x + 16) + 30){
				if (((posSoldier.y + 8) - spikepos.y) > -10 && ((posSoldier.y - 8) - spikepos.y) < 10){
					spikedanger = true;
				}
			}
		}
		if (Game::instance().getKey('s')){
			//cheat: si es manté apretat S els enemics son invulnerables
			spikedanger = false;
		}

		if (alive){
			if (state == IDLE){
				sprite->changeAnimation(RIGHT_STAND);
				busy = true;
				stamp = clock();

				if (posplayer.x + 64.0 > (posSoldier.x + 34) - aggrodistance && posplayer.x + 64.0 < (posSoldier.x + 34) + aggrodistance){
					if (((posplayer.y + 8) - posSoldier.y + 8) > -10 && ((posplayer.y - 8) - posSoldier.y) < 10){
						state = FIGHTING;
					}
				}

			}

			else{
				//state=FIGHTING
				if ((posplayer.x + 64.0 > (posSoldier.x + 34) && posplayer.x + 64.0 < (posSoldier.x + 34) + aggrodistance) && (((posplayer.y + 8) - posSoldier.y + 8) > -10 && ((posplayer.y - 8) - posSoldier.y) < 10)){
					//el príncep està a prop del soldat, a la seva dreta i a la mateixa altura
					if (hp <= 0){						
						sprite->changeAnimation(RIGHT_DIE);
						busy = true;
						stamp = clock();
					}

					else if (takingdamage){
						manager->playEnemyHit();
						if (Game::instance().getKey('s')){
							//cheat: si es manté apretat S els enemics son invulnerables
						}
						else if (Game::instance().getKey('d')){
							//cheat: si es manté apretat D els enemics moren d'un sol atac
							hp -= 100;
						}
						else{
							hp -= 1;
						}
						//cout << "un soldat ha rebut mal\nvida restant= " << hp << "\n";
						sprite->changeAnimation(RIGHT_FLINCH);
						takingdamage = false;
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == RIGHT_STAND){
						if (spikedanger){
							sprite->changeAnimation(RIGHT_SPIKEDEATH);
							busy = true;
							stamp = clock();
						}

						else{
							if ((posplayer.x + 64.0 < (posSoldier.x + 34) + 50)){
								//el príncep està prou a prop per tal d'atacar-lo
								sprite->changeAnimation(RIGHT_ATTACK);
								dealtdamage = false;
								busy = true;
								stamp = clock();
							}
							else{
								//no està prou a prop per atacar, toca acostar-se
								sprite->changeAnimation(RIGHT_STEP_FORWARD);
								busy = true;
								stamp = clock();
							}
						}
					}

					else if (sprite->animation() == RIGHT_STEP_FORWARD){
						sprite->changeAnimation(RIGHT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == RIGHT_STEP_BACK){
						sprite->changeAnimation(RIGHT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == RIGHT_FLINCH){
						sprite->changeAnimation(RIGHT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == RIGHT_ATTACK){
						sprite->changeAnimation(RIGHT_STAND);
						busy = true;
						stamp = clock();
					}
					else{
						sprite->changeAnimation(RIGHT_STAND);
						busy = true;
						stamp = clock();
					}
				}

				else if ((posplayer.x + 64.0 > (posSoldier.x + 34) - aggrodistance && posplayer.x + 64.0 < (posSoldier.x + 34)) && (((posplayer.y + 8) - posSoldier.y + 8) > -10 && ((posplayer.y - 8) - posSoldier.y) < 10)){
					//el príncep està a prop del soldat, a la seva esquerra i a la mateixa altura
					if (hp <= 0){
						sprite->changeAnimation(RIGHT_DIE);
						busy = true;
						stamp = clock();
					}

					else if (takingdamage){
						manager->playEnemyHit();
						if (Game::instance().getKey('s')){
							//cheat: si es manté apretat S els enemics son invulnerables
						}
						else if (Game::instance().getKey('d')){
							//cheat: si es manté apretat D els enemics moren d'un sol atac
							hp -= 100;
						}
						else{
							hp -= 1;
						}
						//cout << "un soldat ha rebut mal\nvida restant= " << hp << "\n";
						sprite->changeAnimation(LEFT_FLINCH);
						takingdamage = false;
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == LEFT_STAND){
						if (spikedanger){
							sprite->changeAnimation(RIGHT_SPIKEDEATH);
							busy = true;
							stamp = clock();
						}
						else{
							if ((posplayer.x + 64.0 > (posSoldier.x + 34) - 50)){
								//el príncep està prou a prop per tal d'atacar-lo
								sprite->changeAnimation(LEFT_ATTACK);
								dealtdamage = false;
								busy = true;
								stamp = clock();
							}
							else{
								//no està prou a prop per atacar, toca acostar-se
								sprite->changeAnimation(LEFT_STEP_FORWARD);
								busy = true;
								stamp = clock();
							}
						}
					}

					else if (sprite->animation() == LEFT_STEP_FORWARD){
						sprite->changeAnimation(LEFT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == LEFT_STEP_BACK){
						sprite->changeAnimation(LEFT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == LEFT_FLINCH){
						sprite->changeAnimation(LEFT_STAND);
						busy = true;
						stamp = clock();
					}

					else if (sprite->animation() == LEFT_ATTACK){
						sprite->changeAnimation(LEFT_STAND);
						busy = true;
						stamp = clock();
					}
					else{
						sprite->changeAnimation(LEFT_STAND);
						busy = true;
						stamp = clock();
					}
				}

				else{
					//si no està en cap de les altres situacions, torna a la posició d'espera
					state = IDLE;
				}
			}
		}
		else{
			if (sprite->animation() != (RIGHT_DEAD) && sprite->animation() != (RIGHT_SPIKEDEATH) && sprite->animation() != (LEFT_SPIKEDEATH)){
				sprite->changeAnimation(RIGHT_DEAD);
				busy = true;
				stamp = clock();
			}
		}


	}//next animation has ben chosen


	//perform actions based on current animation
	else{
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		glm::fvec2 posplayer = player->getPosPlayer();
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\nspikes= "<<spikedanger<<"\n";
		/*if (fall){
		posplayer.y += 62.;
		posplayer.x -= 10.;
		cout << "LALA" << endl;
		fall = false;
		}*/

		if (sprite->animation() == RIGHT_STAND){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_STAND){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_STEP_FORWARD){
			posSoldier.x += 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_STEP_FORWARD){
			posSoldier.x -= 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_STEP_BACK){
			posSoldier.x -= 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_STEP_BACK){
			posSoldier.x += 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_ATTACK){
			if (type == 0){
				if (!dealtdamage){
					if (time >= 7.0 / 8.0 && time < 8.0 / 8.0){
						if ((posplayer.x + 64.0 < (posSoldier.x + 34) + 50)){
							player->takeDamage();
							dealtdamage = true;
						}
					}
				}
				if (time >= 10.9 / 8.0){
					busy = false;
				}
			}
			else if (type == 1){
				if (!dealtdamage){
					if (time >= 4.0 / 8.0 && time < 5.0 / 8.0){
						if ((posplayer.x + 64.0 < (posSoldier.x + 34) + 50)){
							player->takeDamage();
							dealtdamage = true;
						}
					}
				}
				if (time >= 7.9 / 8.0){
					busy = false;
				}
			}
		}

		else if (sprite->animation() == LEFT_ATTACK){
			if (type == 0){
				if (!dealtdamage){
					if (time >= 7.0 / 8.0 && time < 8.0 / 8.0){
						if ((posplayer.x + 64.0 >(posSoldier.x + 34) - 50)){
							player->takeDamage();
							dealtdamage = true;
						}
					}
				}
				if (time >= 10.9 / 8.0){
					busy = false;
				}
			}
			else if (type == 1){
				if (!dealtdamage){
					if (time >= 4.0 / 8.0 && time < 5.0 / 8.0){
						if ((posplayer.x + 64.0 >(posSoldier.x + 34) - 50)){
							player->takeDamage();
							dealtdamage = true;
						}
					}
				}
				if (time >= 7.9 / 8.0){
					busy = false;
				}
			}
		}

		else if (sprite->animation() == RIGHT_FLINCH){
			posSoldier.x -= 8.0 / 8.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FLINCH){
			posSoldier.x += 8.0 / 8.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_DIE){
			if (type == 0){
				manager->playWilhelm();
			}
			else{
				manager->playShogunScream();
			}
			if (time >= 3.9 / 8.0){
				alive = false;
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_DIE){
			if (type == 0){
				manager->playWilhelm();
			}
			else{
				manager->playShogunScream();
			}
			if (time >= 3.9 / 8.0){
				alive = false;
				busy = false;
			}
		}

		if (sprite->animation() == RIGHT_DEAD){
			if (time >= 1.0 / 8.0){
				alive = false;
				busy = false;
			}
		}

		if (sprite->animation() == LEFT_DEAD){
			if (time >= 1.0 / 8.0){
				alive = false;
				busy = false;
			}
		}

		if (sprite->animation() == RIGHT_SPIKEDEATH){
			if (type == 0){
				manager->playWilhelm();
			}
			else{
				manager->playShogunScream();
			}
			if (time >= 1.0 / 8.0){
				alive = false;
				busy = false;
			}
		}

		if (sprite->animation() == LEFT_SPIKEDEATH){
			if (type == 0){
				manager->playWilhelm();
			}
			else{
				manager->playShogunScream();
			}
			if (time >= 1.0 / 8.0){
				alive = false;
				busy = false;
			}
		}


	}



	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSoldier.x), float(tileMapDispl.y + posSoldier.y)));
}

void Soldier::render()
{
	sprite->render();
}


void Soldier::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Soldier::setPosition(const glm::vec2 &pos)
{
	posSoldier = pos;
	sprite->setPosition(glm::vec2(float(posSoldier.x), float(posSoldier.y)));
}

void Soldier::takeDamage()
{
	takingdamage = true;
	busy = false;
}

glm::ivec2 Soldier::getPosRender() {
	return posSoldier;
}

void Soldier::setSpikes(vector<Spikes *> &vec)
{
	spikeTraps = vec;
}

void Soldier::setSoundManager(SoundManager2* man){
	manager = man;
}

bool Soldier::isAlive(){
	return alive;
}

void Soldier::getSliced(){
	if (Game::instance().getKey('s')){
		//cheat: si es manté apretat S els enemics son invulnerables
	}
	else{
		hp -= 100;
		busy = false;
	}
}