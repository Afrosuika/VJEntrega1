#include "Life.h"
#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

//Life::Life()
//{
//}

enum Vides
{
	ZERO_L, ONE_L, TWO_L, THREE_L
};


void Life::init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile("images/IndicadorVida.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(324, 206), glm::vec2(1.0 / 2.0, 1.0 / 2.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(THREE_L, 8);
	sprite->addKeyframe(THREE_L, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(TWO_L, 8);
	sprite->addKeyframe(TWO_L, glm::vec2((1.0f / 2.0f), 0.f));

	sprite->setAnimationSpeed(ONE_L, 8);
	sprite->addKeyframe(ONE_L, glm::vec2(0.f, (1.0f / 2.0f)));

	sprite->setAnimationSpeed(ZERO_L, 8);
	sprite->addKeyframe(ZERO_L, glm::vec2((1.0f / 2.0f), (1.0f / 2.0f)));

	sprite->changeAnimation(THREE_L);
	currentLifes = 3;

	posicioVides = position;
	sprite->setPosition(glm::vec2(position.x + 128, position.y));

	player = pl;

}



void Life::update(int deltaTime)
{
	int nLifes = player->getHp();
	if (nLifes != currentLifes){
		sprite->changeAnimation(nLifes);
		currentLifes = nLifes;
	}
}

void Life::render()
{
	sprite->render();
}

void Life::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Life::setPosition(const glm::vec2 &pos)
{
	posicioVides = pos;
	sprite->setPosition(glm::vec2(posicioVides.x, posicioVides.y - 2.f));
}