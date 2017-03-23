#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT,RIGHT_WINDUP,RIGHT_WINDDOWN
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	busy = false;
	alive = true;
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.2, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.2, 0.f));

		sprite->setAnimationSpeed(RIGHT_WINDUP, 8);
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.2, 0.f));
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.4, 0.f));
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.6, 0.f));

		sprite->setAnimationSpeed(RIGHT_WINDDOWN, 8);
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.4, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.6, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.8, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.0, 0.1f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-1.0, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.2, 0.05f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4, 0.05f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.05f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.05f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
		sprite->update(deltaTime);
		if (!busy){//animation finished, see which triggers next

			if (sprite->animation() == STAND_RIGHT){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(RIGHT_WINDUP);
					busy = true;
					stamp = clock();
				}
			}
			else if (sprite->animation() == RIGHT_WINDUP){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(MOVE_RIGHT);
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(RIGHT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
			else if (sprite->animation() == STAND_LEFT){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(STAND_RIGHT);
					busy = false;
				}
			}
			else if (sprite->animation() == MOVE_RIGHT){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(MOVE_RIGHT);
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(RIGHT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
			else if (sprite->animation() == RIGHT_WINDDOWN){
				sprite->changeAnimation(STAND_RIGHT);
				busy = false;
			}

		}
		else{//perform actions based on current animation
			if (sprite->animation() == RIGHT_WINDUP){
				float time = float(clock() - stamp) / CLOCKS_PER_SEC;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}
			else if (sprite->animation() == MOVE_RIGHT){
				float time = float(clock() - stamp) / CLOCKS_PER_SEC;
				posPlayer.x += 1;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}
			else if (sprite->animation() == RIGHT_WINDDOWN){
				float time = float(clock() - stamp) / CLOCKS_PER_SEC;
				if (time > 4.0 / 8.0){
					busy = false;
				}
			}

		}

		/*if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
		if(sprite->animation() != MOVE_LEFT)
		sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
		posPlayer.x += 2;
		sprite->changeAnimation(STAND_LEFT);
		}
		}
		else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
		if(sprite->animation() != MOVE_RIGHT)
		sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
		posPlayer.x -= 2;
		sprite->changeAnimation(STAND_RIGHT);
		}
		}
		else
		{
		if(sprite->animation() == MOVE_LEFT)
		sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
		sprite->changeAnimation(STAND_RIGHT);
		}*/

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




