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
	STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, RIGHT_WINDUP, LEFT_WINDUP, RIGHT_WINDDOWN, LEFT_WINDDOWN, 
	RIGHT_UNSHEATHE, LEFT_UNSHEATHE, RIGHT_FENCING, LEFT_FENCING
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	busy = false;
	alive = true;
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.2, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);	
		

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.2, 0.f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.05f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.05f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-1.0, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.2, 0.05f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4, 0.05f));

		sprite->setAnimationSpeed(RIGHT_WINDUP, 8);
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.2, 0.f));
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.4, 0.f));
		sprite->addKeyframe(RIGHT_WINDUP, glm::vec2(0.6, 0.f));

		sprite->setAnimationSpeed(LEFT_WINDUP, 8);
		sprite->addKeyframe(LEFT_WINDUP, glm::vec2(-0.4, 0.f));
		sprite->addKeyframe(LEFT_WINDUP, glm::vec2(-0.6, 0.f));
		sprite->addKeyframe(LEFT_WINDUP, glm::vec2(-0.8, 0.f));

		sprite->setAnimationSpeed(RIGHT_WINDDOWN, 8);
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.4, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.6, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.8, 0.05f));
		sprite->addKeyframe(RIGHT_WINDDOWN, glm::vec2(0.0, 0.1f));

		sprite->setAnimationSpeed(LEFT_WINDDOWN, 8);
		sprite->addKeyframe(LEFT_WINDDOWN, glm::vec2(-0.6, 0.05f));
		sprite->addKeyframe(LEFT_WINDDOWN, glm::vec2(-0.8, 0.05f));
		sprite->addKeyframe(LEFT_WINDDOWN, glm::vec2(-1.0, 0.05f));
		sprite->addKeyframe(LEFT_WINDDOWN, glm::vec2(-0.2, 0.1f));

		sprite->setAnimationSpeed(RIGHT_UNSHEATHE, 8);
		sprite->addKeyframe(RIGHT_UNSHEATHE, glm::vec2(0.2, 0.1f));
		sprite->addKeyframe(RIGHT_UNSHEATHE, glm::vec2(0.4, 0.1f));
		sprite->addKeyframe(RIGHT_UNSHEATHE, glm::vec2(0.6, 0.1f));

		sprite->setAnimationSpeed(LEFT_UNSHEATHE, 8);
		sprite->addKeyframe(LEFT_UNSHEATHE, glm::vec2(-0.4, 0.1f));
		sprite->addKeyframe(LEFT_UNSHEATHE, glm::vec2(-0.6, 0.1f));
		sprite->addKeyframe(LEFT_UNSHEATHE, glm::vec2(-0.8, 0.1f));

		sprite->setAnimationSpeed(RIGHT_FENCING, 8);
		sprite->addKeyframe(RIGHT_FENCING, glm::vec2(0.8f, 0.1f));

		sprite->setAnimationSpeed(LEFT_FENCING, 8);
		sprite->addKeyframe(LEFT_FENCING, glm::vec2(-1.0f, 0.1f));
		
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
		sprite->update(deltaTime);
		if (!busy){//current animation finished, see which triggers next

			if (sprite->animation() == STAND_RIGHT){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(RIGHT_WINDUP);
					busy = true;
					stamp = clock();
				}
				else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (Game::instance().getKey('x')){
					sprite->changeAnimation(RIGHT_UNSHEATHE);
					busy = true;
					stamp = clock();
				}
			}

			else if (sprite->animation() == STAND_LEFT){
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
					sprite->changeAnimation(LEFT_WINDUP);
					busy = true;
					stamp = clock();
				}
				else if (Game::instance().getKey('x')){
					sprite->changeAnimation(LEFT_UNSHEATHE);
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

			else if (sprite->animation() == LEFT_WINDUP){
				if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
					sprite->changeAnimation(MOVE_LEFT);
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(LEFT_WINDDOWN);
					busy = true;
					stamp = clock();
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

			else if (sprite->animation() == MOVE_LEFT){
				if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
					sprite->changeAnimation(MOVE_LEFT);
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(LEFT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}

			else if (sprite->animation() == RIGHT_WINDDOWN){
				sprite->changeAnimation(STAND_RIGHT);
				busy = true;
				stamp = clock();
			}

			else if (sprite->animation() == LEFT_WINDDOWN){
				sprite->changeAnimation(STAND_LEFT);
				busy = true;
				stamp = clock();
			}

			else if (sprite->animation() == RIGHT_UNSHEATHE){
				sprite->changeAnimation(RIGHT_FENCING);
				busy = true;
				stamp = clock();
			}

			else if (sprite->animation() == LEFT_UNSHEATHE){
				sprite->changeAnimation(LEFT_FENCING);
				busy = true;
				stamp = clock();
			}

			else if (sprite->animation() == RIGHT_FENCING){
				
			}

			else if (sprite->animation() == LEFT_FENCING){

			}





		}
		else{//perform actions based on current animation
			float time = float(clock() - stamp) / CLOCKS_PER_SEC;
			cout << "entra a busy\n"<<time<<"\nanimacio="<<sprite->animation()<<"\n";
			if (sprite->animation() == STAND_RIGHT){
				if (time > 1.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == STAND_LEFT){
				if (time > 1.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == RIGHT_WINDUP){
				posPlayer.x += 1.0;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == LEFT_WINDUP){
				posPlayer.x -= 1.0;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == MOVE_RIGHT){
				posPlayer.x += 1.0;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == MOVE_LEFT){
				posPlayer.x -= 1.0;
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == RIGHT_WINDDOWN){
				posPlayer.x += 3.0/4.0;
				if (time > 4.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == LEFT_WINDDOWN){
				posPlayer.x -= 3.0/4.0;
				if (time > 4.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == RIGHT_UNSHEATHE){
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == LEFT_UNSHEATHE){
				if (time > 3.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == RIGHT_FENCING){
				if (time > 1.0 / 8.0){
					busy = false;
				}
			}

			else if (sprite->animation() == LEFT_FENCING){
				if (time > 1.0 / 8.0){
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




