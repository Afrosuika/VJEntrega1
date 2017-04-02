#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Spikes.h"
#include "Soldier.h"
#include "Portagran.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_RIGHT, MOVE_LEFT, RIGHT_WINDUP, LEFT_WINDUP, RIGHT_WINDDOWN, LEFT_WINDDOWN,
	RIGHT_UNSHEATHE, LEFT_UNSHEATHE, RIGHT_FENCING, LEFT_FENCING, RIGHT_FENCING_STEPFWRD, LEFT_FENCING_STEPFWRD,
	RIGHT_FENCING_STEPBACK, LEFT_FENCING_STEPBACK, RIGHT_SHEATHE, LEFT_SHEATHE, RIGHT_ATTACK, LEFT_ATTACK,
	RIGHT_SMALLSTEP, LEFT_SMALLSTEP, RIGHT_JUMPUP, LEFT_JUMPUP, RIGHT_LAND, LEFT_LAND, RIGHT_GRAB, LEFT_GRAB,
	RIGHT_CLIMB, LEFT_CLIMB, RIGHT_JUMPFWRD, LEFT_JUMPFWRD, ENTER_BIGDOOR, GONE, RIGHT_FWRDLAND, LEFT_FWRDLAND,
	RIGHT_DEATH, LEFT_DEATH, RIGHT_SPIKEDEATH, LEFT_SPIKEDEATH, RIGHT_FALL, LEFT_FALL, RIGHT_STRAIGHTFALL,
	LEFT_STRAIGHTFALL, RIGHT_ARISE, LEFT_ARISE, RIGHT_VOLTERETA, LEFT_VOLTERETA
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	isLevelFinished = false;
	fall = false;
	bJumping = false;
	busy = false;
	alive = true;
	spikedanger = false;
	potacabarnivell = false;
	manager = new SoundManager2();
	hp = 3;
	dealtdamage = false;
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(128, 64), glm::vec2(0.2, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(48);


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

	sprite->setAnimationSpeed(RIGHT_FENCING_STEPFWRD, 8);
	sprite->addKeyframe(RIGHT_FENCING_STEPFWRD, glm::vec2(0.4f, 0.6f));

	sprite->setAnimationSpeed(LEFT_FENCING_STEPFWRD, 8);
	sprite->addKeyframe(LEFT_FENCING_STEPFWRD, glm::vec2(-0.6f, 0.6f));

	sprite->setAnimationSpeed(RIGHT_FENCING_STEPBACK, 8);
	sprite->addKeyframe(RIGHT_FENCING_STEPBACK, glm::vec2(0.4f, 0.6f));

	sprite->setAnimationSpeed(LEFT_FENCING_STEPBACK, 8);
	sprite->addKeyframe(LEFT_FENCING_STEPBACK, glm::vec2(-0.6f, 0.6f));

	sprite->setAnimationSpeed(RIGHT_SHEATHE, 8);
	sprite->addKeyframe(RIGHT_SHEATHE, glm::vec2(0.6, 0.1f));
	sprite->addKeyframe(RIGHT_SHEATHE, glm::vec2(0.4, 0.1f));
	sprite->addKeyframe(RIGHT_SHEATHE, glm::vec2(0.2, 0.1f));

	sprite->setAnimationSpeed(LEFT_SHEATHE, 8);
	sprite->addKeyframe(LEFT_SHEATHE, glm::vec2(-0.8, 0.1f));
	sprite->addKeyframe(LEFT_SHEATHE, glm::vec2(-0.6, 0.1f));
	sprite->addKeyframe(LEFT_SHEATHE, glm::vec2(-0.4, 0.1f));

	sprite->setAnimationSpeed(RIGHT_ATTACK, 8);
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.0, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.2, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.6, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.8, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.4, 0.15f));
	sprite->addKeyframe(RIGHT_ATTACK, glm::vec2(0.0, 0.15f));

	sprite->setAnimationSpeed(LEFT_ATTACK, 8);
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.4, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.8, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-1.0, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.6, 0.15f));
	sprite->addKeyframe(LEFT_ATTACK, glm::vec2(-0.2, 0.15f));

	sprite->setAnimationSpeed(RIGHT_SMALLSTEP, 8);
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.2, 0.2f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.4, 0.2f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.6, 0.2f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.8, 0.2f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.0, 0.25f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.2, 0.25f));
	sprite->addKeyframe(RIGHT_SMALLSTEP, glm::vec2(0.4, 0.25f));

	sprite->setAnimationSpeed(LEFT_SMALLSTEP, 8);
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.4, 0.2f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.6, 0.2f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.8, 0.2f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-1.0, 0.2f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.2, 0.25f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.4, 0.25f));
	sprite->addKeyframe(LEFT_SMALLSTEP, glm::vec2(-0.6, 0.25f));

	sprite->setAnimationSpeed(RIGHT_JUMPUP, 8);
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.6, 0.25f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.8, 0.25f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.0, 0.3f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.2, 0.3f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.4, 0.3f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.6, 0.3f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.8, 0.3f));
	sprite->addKeyframe(RIGHT_JUMPUP, glm::vec2(0.0, 0.35f));

	sprite->setAnimationSpeed(LEFT_JUMPUP, 8);
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.8, 0.25f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-1.0, 0.25f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.2, 0.3f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.4, 0.3f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.6, 0.3f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.8, 0.3f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-1.0, 0.3f));
	sprite->addKeyframe(LEFT_JUMPUP, glm::vec2(-0.2, 0.35f));

	sprite->setAnimationSpeed(RIGHT_LAND, 8);
	sprite->addKeyframe(RIGHT_LAND, glm::vec2(0.2, 0.35f));
	sprite->addKeyframe(RIGHT_LAND, glm::vec2(0.2, 0.35f));
	sprite->addKeyframe(RIGHT_LAND, glm::vec2(0.4, 0.35f));

	sprite->setAnimationSpeed(LEFT_LAND, 8);
	sprite->addKeyframe(LEFT_LAND, glm::vec2(-0.4, 0.35f));
	sprite->addKeyframe(LEFT_LAND, glm::vec2(-0.4, 0.35f));
	sprite->addKeyframe(LEFT_LAND, glm::vec2(-0.6, 0.35f));

	sprite->setAnimationSpeed(RIGHT_GRAB, 8);
	sprite->addKeyframe(RIGHT_GRAB, glm::vec2(0.6f, 0.35f));

	sprite->setAnimationSpeed(LEFT_GRAB, 8);
	sprite->addKeyframe(LEFT_GRAB, glm::vec2(-0.8f, 0.35f));

	sprite->setAnimationSpeed(RIGHT_CLIMB, 8);
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.8f, 0.35f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.0f, 0.45f));
	sprite->addKeyframe(RIGHT_CLIMB, glm::vec2(0.2f, 0.45f));

	sprite->setAnimationSpeed(LEFT_CLIMB, 8);
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-1.0f, 0.35f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.4f, 0.4f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.6f, 0.4f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.8f, 0.4f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-1.0f, 0.4f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.2f, 0.45f));
	sprite->addKeyframe(LEFT_CLIMB, glm::vec2(-0.4f, 0.45f));

	sprite->setAnimationSpeed(RIGHT_JUMPFWRD, 8);
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.4, 0.45f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.6, 0.45f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.8, 0.45f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.0, 0.5f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.2, 0.5f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.4, 0.5f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.6, 0.5f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.8, 0.5f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.0, 0.55f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.2, 0.55f));
	sprite->addKeyframe(RIGHT_JUMPFWRD, glm::vec2(0.4, 0.55f));

	sprite->setAnimationSpeed(RIGHT_FWRDLAND, 8);
	sprite->addKeyframe(RIGHT_FWRDLAND, glm::vec2(0.6, 0.55f));
	sprite->addKeyframe(RIGHT_FWRDLAND, glm::vec2(0.8, 0.55f));
	sprite->addKeyframe(RIGHT_FWRDLAND, glm::vec2(0.0, 0.6f));
	sprite->addKeyframe(RIGHT_FWRDLAND, glm::vec2(0.2, 0.6f));

	sprite->setAnimationSpeed(LEFT_JUMPFWRD, 8);
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.6, 0.45f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.8, 0.45f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-1.0, 0.45f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.2, 0.5f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.4, 0.5f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.6, 0.5f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.8, 0.5f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-1.0, 0.5f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.2, 0.55f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.4, 0.55f));
	sprite->addKeyframe(LEFT_JUMPFWRD, glm::vec2(-0.6, 0.55f));

	sprite->setAnimationSpeed(LEFT_FWRDLAND, 8);
	sprite->addKeyframe(LEFT_FWRDLAND, glm::vec2(-0.8, 0.55f));
	sprite->addKeyframe(LEFT_FWRDLAND, glm::vec2(-1.0, 0.55f));
	sprite->addKeyframe(LEFT_FWRDLAND, glm::vec2(-0.2, 0.6f));
	sprite->addKeyframe(LEFT_FWRDLAND, glm::vec2(-0.4, 0.6f));

	sprite->setAnimationSpeed(ENTER_BIGDOOR, 8);
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.0, 0.65f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.2, 0.65f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.4, 0.65f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.6, 0.65f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.8, 0.65f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.0, 0.7f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.2, 0.7f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.4, 0.7f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.6, 0.7f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.8, 0.7f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(ENTER_BIGDOOR, glm::vec2(0.f, 0.8f));

	sprite->setAnimationSpeed(GONE, 8);
	sprite->addKeyframe(GONE, glm::vec2(0.f, 0.8f));

	sprite->setAnimationSpeed(RIGHT_DEATH, 8);
	sprite->addKeyframe(RIGHT_DEATH, glm::vec2(0.6f, 0.6f));

	sprite->setAnimationSpeed(LEFT_DEATH, 8);
	sprite->addKeyframe(LEFT_DEATH, glm::vec2(-0.8f, 0.6f));

	sprite->setAnimationSpeed(RIGHT_SPIKEDEATH, 8);
	sprite->addKeyframe(RIGHT_SPIKEDEATH, glm::vec2(0.8f, 0.6f));

	sprite->setAnimationSpeed(LEFT_SPIKEDEATH, 8);
	sprite->addKeyframe(LEFT_SPIKEDEATH, glm::vec2(-1.0f, 0.6f));

	sprite->setAnimationSpeed(RIGHT_FALL, 8);
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.2f, 0.4f));
	/*sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(RIGHT_FALL, glm::vec2(0.6f, 0.4f));*/
	

	sprite->setAnimationSpeed(LEFT_FALL, 8);
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.4f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.4f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.4f, 0.4f));
	/*sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.6f, 0.4f));
	sprite->addKeyframe(LEFT_FALL, glm::vec2(-0.8f, 0.4f));*/
	

	sprite->setAnimationSpeed(RIGHT_ARISE, 8);
	sprite->addKeyframe(RIGHT_ARISE, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(RIGHT_ARISE, glm::vec2(0.0f, 0.45f));
	sprite->addKeyframe(RIGHT_ARISE, glm::vec2(0.2f, 0.45f));

	sprite->setAnimationSpeed(LEFT_ARISE, 8);
	sprite->addKeyframe(LEFT_ARISE, glm::vec2(-1.0f, 0.4f));
	sprite->addKeyframe(LEFT_ARISE, glm::vec2(-0.2f, 0.45f));
	sprite->addKeyframe(LEFT_ARISE, glm::vec2(-0.4f, 0.45f));

	sprite->setAnimationSpeed(RIGHT_STRAIGHTFALL, 8);
	sprite->addKeyframe(RIGHT_STRAIGHTFALL, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(LEFT_STRAIGHTFALL, 8);
	sprite->addKeyframe(LEFT_STRAIGHTFALL, glm::vec2(-0.4f, 0.4f));

	//sprite->RIGHT_VOLTERETA
	sprite->setAnimationSpeed(RIGHT_VOLTERETA, 10);		
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.8, 0.45f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0, 0.5f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0f, 0.75f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.6f, 0.75f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.8f, 0.75f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0f, 0.8f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.2f, 0.8f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.4f, 0.8f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.8f, 0.8f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0f, 0.85f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.2f, 0.85f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.4f, 0.85f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.6f, 0.85f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.8f, 0.85f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0f, 0.9f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.2f, 0.9f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.4f, 0.9f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.6f, 0.9f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.8f, 0.9f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.0f, 0.95f));
	sprite->addKeyframe(RIGHT_VOLTERETA, glm::vec2(0.2f, 0.95f));

	//LEFT_VOLTERETA
	sprite->setAnimationSpeed(LEFT_VOLTERETA, 10);
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-1.0, 0.45f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2, 0.5f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2f, 0.75f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.4f, 0.75f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.6f, 0.75f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.8f, 0.75f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-1.0f, 0.75f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2f, 0.8f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.4f, 0.8f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.6f, 0.8f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.8f, 0.8f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-1.0f, 0.8f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2f, 0.85f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.4f, 0.85f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.6f, 0.85f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.7f, 0.85f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-1.0f, 0.85f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2f, 0.9f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.4f, 0.9f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.6f, 0.9f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.8f, 0.9f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-1.0f, 0.9f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.2f, 0.95f));
	sprite->addKeyframe(LEFT_VOLTERETA, glm::vec2(-0.4f, 0.95f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!busy){//current animation finished, see which triggers next

		spikedanger = false;//primer comprobem si està a sobre d'unes punxes
		for (Spikes* theSpike : spikeTraps){
			glm::ivec2 spikepos = theSpike->getPosRender();
			if (posPlayer.x + 64.0 > (spikepos.x + 16) - 10 && posPlayer.x + 64.0 < (spikepos.x + 16) + 30){
				if (((posPlayer.y + 8) - spikepos.y) > -10 && ((posPlayer.y - 8) - spikepos.y) < 10){
					spikedanger = true;
				}

			}
		}


		if (Game::instance().getKey('a')){
			//cheat: si es manté apretat A el príncep és invulnerable
			spikedanger = false;
		}

		glm::ivec2 portagranpos = portagran->getPosRender();
		if (posPlayer.x + 64.0 > (portagranpos.x + 60) - 20 && posPlayer.x + 64.0 < (portagranpos.x + 60) + 20){			
			if (((posPlayer.y + 8) - portagranpos.y-64) > -10 && ((posPlayer.y - 8) - portagranpos.y-64) < 10){				
				if (portagran->getOberta()){
					potacabarnivell = true;
				}
			}
		}
		else{
			potacabarnivell = false;
		}


		if (hp <= 0){
			sprite->changeAnimation(RIGHT_DEATH);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_VOLTERETA){
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				sprite->changeAnimation(STAND_RIGHT);
				busy = true;
				stamp = clock();
			}
			else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(RIGHT_FALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(RIGHT_FWRDLAND);
				stamp = clock();
				busy = true;
			}
		}

		else if (sprite->animation() == LEFT_VOLTERETA){
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
			{
				sprite->changeAnimation(STAND_LEFT);
				busy = true;
				stamp = clock();
			}
			else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(LEFT_FALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(LEFT_FWRDLAND);
				stamp = clock();
				busy = true;
			}
		}

		else if (sprite->animation() == STAND_RIGHT){
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getKey('z') && !Game::instance().getSpecialKey(GLUT_KEY_UP)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_WINDUP);
					busy = true;
					stamp = clock();
				}
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
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getKey('z')){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_SMALLSTEP);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getKey('v') && !potacabarnivell){
				//cheat: apretar la v simula estar davant la porta gran
				startY = posPlayer.y;
				sprite->changeAnimation(RIGHT_JUMPUP);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)){
				startY = posPlayer.y;
				sprite->changeAnimation(RIGHT_JUMPFWRD);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && (Game::instance().getKey('v') || potacabarnivell)){
				//cheat: apretar la v simula estar davant la porta gran
				sprite->changeAnimation(ENTER_BIGDOOR);
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
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)){
				startY = posPlayer.y;
				sprite->changeAnimation(LEFT_JUMPFWRD);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getKey('z') && !Game::instance().getSpecialKey(GLUT_KEY_UP)){

				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_WINDUP);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getKey('x')){
				sprite->changeAnimation(LEFT_UNSHEATHE);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getKey('z')){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_SMALLSTEP);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Game::instance().getKey('v') && !potacabarnivell){
				//cheat: apretar la v simula estar davant la porta gran
				startY = posPlayer.y;				
				sprite->changeAnimation(LEFT_JUMPUP);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && (Game::instance().getKey('v') || potacabarnivell)){
				startY = posPlayer.y;
				//cheat: apretar la v simula estar davant la porta gran
				sprite->changeAnimation(ENTER_BIGDOOR);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == RIGHT_WINDUP){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(MOVE_RIGHT);
					busy = true;
					stamp = clock();
				}
			}
			else{
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
		}

		else if (sprite->animation() == LEFT_WINDUP){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(MOVE_LEFT);
					busy = true;
					stamp = clock();
				}
			}
			else{
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
		}

		else if (sprite->animation() == MOVE_RIGHT){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_VOLTERETA);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(MOVE_RIGHT);
					busy = true;
					stamp = clock();
				}
			}
			else{
				//AIXO SEMBLA NO FUNCIONAR
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_RIGHT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				//FINS AQUI
				else {
					sprite->changeAnimation(RIGHT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
		}

		else if (sprite->animation() == MOVE_LEFT){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_VOLTERETA);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(64, 64), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				else{
					sprite->changeAnimation(MOVE_LEFT);
					busy = true;
					stamp = clock();
				}
			}
			else{
				//AIXO SEMBLA NO FUNCIONAR
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(STAND_LEFT);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(64, 64), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FALL);
					startY = posPlayer.y;
					busy = true;
					stamp = clock();
				}
				//FINS AQUI
				else{
					sprite->changeAnimation(LEFT_WINDDOWN);
					busy = true;
					stamp = clock();
				}
			}
		}

		else if (sprite->animation() == RIGHT_WINDDOWN){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			//AIXO SEMBLA NO-FUNCIONAR
			else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(RIGHT_FALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				sprite->changeAnimation(STAND_RIGHT);
				busy = true;
				stamp = clock();
			}
			//FINS AQUI
			else{
				sprite->changeAnimation(STAND_RIGHT);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == LEFT_WINDDOWN){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(LEFT_FALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
				busy = true;
				stamp = clock();
			}
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
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(RIGHT_FENCING);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FENCING);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_FENCING_STEPFWRD);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(RIGHT_FENCING);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(RIGHT_FENCING);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(RIGHT_FENCING_STEPBACK);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
				sprite->changeAnimation(RIGHT_SHEATHE);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getKey('x')){
				sprite->changeAnimation(RIGHT_ATTACK);
				dealtdamage = false;
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == LEFT_FENCING){
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(LEFT_FENCING);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FENCING);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_FENCING_STEPBACK);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				{
					sprite->changeAnimation(LEFT_FENCING);
					busy = true;
					stamp = clock();
				}
				else if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
					sprite->changeAnimation(LEFT_FENCING);
					busy = true;
					stamp = clock();
				}
				else {
					sprite->changeAnimation(LEFT_FENCING_STEPFWRD);
					busy = true;
					stamp = clock();
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
				sprite->changeAnimation(LEFT_SHEATHE);
				busy = true;
				stamp = clock();
			}
			else if (Game::instance().getKey('x')){
				sprite->changeAnimation(LEFT_ATTACK);
				dealtdamage = false;
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == RIGHT_FENCING_STEPFWRD){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(RIGHT_FENCING);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == RIGHT_FENCING_STEPBACK){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else{
				sprite->changeAnimation(RIGHT_FENCING);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == LEFT_FENCING_STEPFWRD){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else{
				sprite->changeAnimation(LEFT_FENCING);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == LEFT_FENCING_STEPBACK){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else{
				sprite->changeAnimation(LEFT_FENCING);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == RIGHT_SHEATHE){
			sprite->changeAnimation(STAND_RIGHT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_SHEATHE){
			sprite->changeAnimation(STAND_LEFT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_ATTACK){
			sprite->changeAnimation(RIGHT_FENCING);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_ATTACK){
			sprite->changeAnimation(LEFT_FENCING);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_SMALLSTEP){
			sprite->changeAnimation(STAND_RIGHT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_SMALLSTEP){
			sprite->changeAnimation(STAND_LEFT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_JUMPUP){
			if (map->collisionClimbRight(posPlayer)) {
				//cout << "OH YES" << endl;
				sprite->changeAnimation(RIGHT_GRAB);
			}
			else if (Game::instance().getKey('c')){
				//cheat: apretar la c simula que hi ha un lloc on agafar-se
				sprite->changeAnimation(RIGHT_GRAB);
			}
			else{
				sprite->changeAnimation(RIGHT_LAND);
			}
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_JUMPUP){
			if (map->collisionClimbLeft(posPlayer)) {
				//cout << "OH YES" << endl;
				sprite->changeAnimation(LEFT_GRAB);
			}
			else if (Game::instance().getKey('c')){
				//cheat: apretar la c simula que hi ha un lloc on agafar-se
				sprite->changeAnimation(LEFT_GRAB);
			}
			else{
				sprite->changeAnimation(LEFT_LAND);
			}
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_LAND){
			sprite->changeAnimation(STAND_RIGHT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_LAND){
			sprite->changeAnimation(STAND_LEFT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_GRAB){
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
				sprite->changeAnimation(RIGHT_LAND);
				busy = true;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
				sprite->changeAnimation(RIGHT_CLIMB);
				busy = true;
			}
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_GRAB){
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
				sprite->changeAnimation(LEFT_LAND);
				busy = true;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
				sprite->changeAnimation(LEFT_CLIMB);
				busy = true;
			}
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_CLIMB){
			sprite->changeAnimation(STAND_RIGHT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_CLIMB){
			sprite->changeAnimation(STAND_LEFT);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == RIGHT_JUMPFWRD){
			if (map->collisionMoveRight(posPlayer, glm::vec2(32, 32))){
				sprite->changeAnimation(STAND_RIGHT);
			}
			else 
				sprite->changeAnimation(RIGHT_FWRDLAND);
			busy = true;
			stamp = clock();
		}

		else if (sprite->animation() == LEFT_JUMPFWRD){
			if (map->collisionMoveLeft(posPlayer, glm::vec2(32, 32))){
				sprite->changeAnimation(STAND_LEFT);
			}
			else
				sprite->changeAnimation(LEFT_FWRDLAND);
			busy = true;
			stamp = clock();
		}
		else if (sprite->animation() == RIGHT_FWRDLAND){
			if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(STAND_RIGHT);
				busy = true;
				stamp = clock();
			}
		}

		else if (sprite->animation() == LEFT_FWRDLAND){
			if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
				busy = true;
				stamp = clock();
			}
		}
		else if (sprite->animation() == ENTER_BIGDOOR){
			sprite->changeAnimation(GONE);
			busy = true;
			stamp = clock();
			manager->playEndLevel();
			//cout << "aqui es faria trigger de \"nextlevel\" o algo aixi \n";
			isLevelFinished = true;
			
		}
		else if (sprite->animation() == RIGHT_FALL){
			if (map->collisionMoveDownRight(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(RIGHT_STRAIGHTFALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(RIGHT_ARISE);
				busy = true;
				stamp = clock();
			}
		}
		else if (sprite->animation() == LEFT_FALL){
			if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(LEFT_STRAIGHTFALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else if (spikedanger){
				sprite->changeAnimation(LEFT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(LEFT_ARISE);
				busy = true;
				stamp = clock();
			}
		}
		else if (sprite->animation() == RIGHT_STRAIGHTFALL){
			if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(RIGHT_STRAIGHTFALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(RIGHT_ARISE);
				busy = true;
				stamp = clock();
			}
		}
		else if (sprite->animation() == LEFT_STRAIGHTFALL){
			if (map->collisionMoveDownLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
				sprite->changeAnimation(LEFT_STRAIGHTFALL);
				startY = posPlayer.y;
				busy = true;
				stamp = clock();
			}
			else if (spikedanger){
				sprite->changeAnimation(RIGHT_SPIKEDEATH);
				busy = true;
				stamp = clock();
			}
			else {
				sprite->changeAnimation(LEFT_ARISE);
				busy = true;
				stamp = clock();
			}
		}
		else if (sprite->animation() == RIGHT_ARISE){
			sprite->changeAnimation(STAND_RIGHT);
			busy = true;
			stamp = clock();
		}
		else if (sprite->animation() == LEFT_ARISE){
			sprite->changeAnimation(STAND_LEFT);
			busy = true;
			stamp = clock();
		}


	}//next animation has ben chosen


	//perform actions based on current animation
	else{
		float time = float(clock() - stamp) / CLOCKS_PER_SEC;
		//cout << "entra a busy\n" << time << "\nanimacio=" << sprite->animation() << "\nspikes= "<<spikedanger<<"\n";
		/*if (fall){
		posPlayer.y += 62.;
		posPlayer.x -= 10.;
		cout << "LALA" << endl;
		fall = false;
		}*/

		if (sprite->animation() == STAND_RIGHT){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == STAND_LEFT){
			//cout << "STAND LEFT" << endl;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_WINDUP){
			manager->playPrinceSteps();
			posPlayer.x += 1.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_WINDUP){
			manager->playPrinceSteps();
			posPlayer.x -= 1.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == MOVE_RIGHT){
			manager->playPrinceSteps();
			posPlayer.x += 1.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == MOVE_LEFT){
			manager->playPrinceSteps();
			posPlayer.x -= 1.0;
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_WINDDOWN){
			manager->playPrinceSteps();
			posPlayer.x += 3.0 / 4.0;
			if (time >= 3.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_WINDDOWN){
			manager->playPrinceSteps();
			posPlayer.x -= 3.0 / 4.0;
			if (time >= 3.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_UNSHEATHE){
			manager->playPrinceUnsheathe();
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_UNSHEATHE){
			manager->playPrinceUnsheathe();
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_FENCING){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FENCING){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_FENCING_STEPFWRD){
			posPlayer.x += 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_FENCING_STEPBACK){
			posPlayer.x -= 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FENCING_STEPFWRD){
			posPlayer.x -= 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FENCING_STEPBACK){
			posPlayer.x += 5.0 / 8.0;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_SHEATHE){
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_SHEATHE){
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_ATTACK){
			if (!dealtdamage){
				if (time >= 4.0 / 8.0 && time < 5.0 / 8.0){
					manager->playPrinceSlash();
					for (Soldier* soldat : soldiers){
						glm::fvec2 possoldier = soldat->getPosRender();
						if ((possoldier.x + 34) < posPlayer.x + 64 + 55){
							if (((posPlayer.y + 8) - possoldier.y) > -10 && ((posPlayer.y - 8) - possoldier.y) < 10){
								soldat->takeDamage();
								dealtdamage = true;
							}
						}
					}
				}
			}
			if (time >= 6.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_ATTACK){
			if (!dealtdamage){
				if (time >= 4.0 / 8.0 && time < 5.0 / 8.0){
					manager->playPrinceSlash();
					for (Soldier* soldat : soldiers){
						glm::fvec2 possoldier = soldat->getPosRender();
						if ((possoldier.x + 34) > posPlayer.x + 64 - 55){
							if (((posPlayer.y + 8) - possoldier.y) > -10 && ((posPlayer.y - 8) - possoldier.y) < 10){
								soldat->takeDamage();
								dealtdamage = true;
							}
						}
					}
				}
			}
			if (time >= 6.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_SMALLSTEP){
			if (time >= 3.0 / 8.0){
				posPlayer.x += 1.0*(4.0 / 8.0);
			}
			if (time >= 6.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_SMALLSTEP){
			if (time >= 3.0 / 8.0){
				posPlayer.x -= 1.0*(4.0 / 8.0);
			}
			if (time >= 6.9 / 8.0){
				busy = false;
			}
		}


		else if (sprite->animation() == RIGHT_JUMPUP){
			if (time >= 5.0 / 8.0  && time < 6.9 / 8.0){
				posPlayer.y -= 1.0*(11.5 / 8.0);
			}
			if (time >= 7.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_JUMPUP){
			if (time >= 5.0 / 8.0  && time < 6.9 / 8.0){
				posPlayer.y -= 1.0*(11.5 / 8.0);
			}
			if (time >= 7.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_LAND){
			if (time<2.0 / 8.0){
				posPlayer.y += 1.0*(11.5 / 8.0);
			}
			if (time >= 2.9 / 8.0){
				posPlayer.y = startY;
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_LAND){
			if (time<2.0 / 8.0){
				posPlayer.y += 1.0*(11.5 / 8.0);
			}
			if (time >= 2.9 / 8.0){
				posPlayer.y = startY;
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_GRAB){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_GRAB){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_CLIMB){
			if (time >= 1.0 / 8.0  && time < 4.0 / 8.0){
				posPlayer.y -= 16.0 / 8.0;
			}
			if (time >= 3.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.x += 1.0*(3.0 / 8.0);
			}
			if (time >= 7.9 / 8.0){
				posPlayer.y = startY - 64;
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_CLIMB){
			if (time >= 1.0 / 8.0  && time < 4.0 / 8.0){
				posPlayer.y -= 16.0 / 8.0;
			}
			if (time >= 3.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.x -= 1.0*(3.0 / 8.0);
			}
			if (time >= 7.9 / 8.0){
				posPlayer.y = startY - 64;
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_JUMPFWRD){
			if (map->collisionMoveRight(posPlayer, glm::vec2(32, 32))){
				posPlayer.y = startY;
				busy = false;
			}
			else {
				if (time >= 5.0 / 8.0  && time < 7.0 / 8.0){
					posPlayer.y -= 4.0 / 8.0;
				}
				else if (time >= 8.0 / 8.0  && time < 10.0 / 8.0){
					posPlayer.y += 4.0 / 8.0;
				}

				if (time >= 5.0 / 8.0  && time < 10.0 / 8.0){
					posPlayer.x += 1.0*(14.0 / 8.0);
				}

				if (time >= 9.9 / 8.0){
					posPlayer.y = startY;
					busy = false;
				}
			}
		}

		else if (sprite->animation() == LEFT_JUMPFWRD){
			if (map->collisionMoveLeft(posPlayer, glm::vec2(32, 32))){
				posPlayer.y = startY;
				busy = false;
			}
			else {
				if (time >= 5.0 / 8.0  && time < 7.0 / 8.0){
					posPlayer.y -= 4.0 / 8.0;
				}
				else if (time >= 8.0 / 8.0  && time < 10.0 / 8.0){
					posPlayer.y += 4.0 / 8.0;
				}

				if (time >= 5.0 / 8.0  && time < 10.0 / 8.0){
					posPlayer.x -= 1.0*(14.0 / 8.0);
				}

				if (time >= 9.9 / 8.0){
					posPlayer.y = startY;
					busy = false;
				}
			}
		}

		else if (sprite->animation() == RIGHT_FWRDLAND){
			manager->playPrinceSteps();
			if (time >= 0.0 / 8.0){
				posPlayer.x += 1.0*(6.0 / 8.0);
			}
			if (time >= 3.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FWRDLAND){
			manager->playPrinceSteps();
			if (time >= 0.0 / 8.0){
				posPlayer.x -= 1.0*(6.0 / 8.0);
			}
			if (time >= 3.9 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == ENTER_BIGDOOR){
			manager->playPrinceSteps();
			if (time >= 10.9 / 8.0){				
				busy = false;
			}
		}

		else if (sprite->animation() == GONE){
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_DEATH){
			alive = false;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_SPIKEDEATH){
			alive = false;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_SPIKEDEATH){
			alive = false;
			if (time >= 1.0 / 8.0){
				busy = false;
			}
		}

		else if (sprite->animation() == RIGHT_FALL){ //en total, y+=64, x-=10
			if (time >= 1.0 / 8.0  && time < 4.0 / 8.0){
				posPlayer.x += 7.0 / 8.0;
			}
			if (time >= 3.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.y += 22.9 / 8.0;
			}
			if (time >= 6.1 / 8.0){
				posPlayer.y = startY + 64;
				busy = false;
			}
		}

		else if (sprite->animation() == LEFT_FALL){ //en total, y+=64, x-=10
			if (time >= 1.0 / 8.0  && time < 4.0 / 8.0){
				posPlayer.x -= 7.0 / 8.0;
			}
			if (time >= 3.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.y += 22.9 / 8.0;
			}
			if (time >= 6.1 / 8.0){
				posPlayer.y = startY + 64;
				busy = false;
			}
		}
		
		else if (sprite->animation() == RIGHT_STRAIGHTFALL){
			if (time >= 1.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.y += 14. / 8.0;
			}
			if (time >= 7.9 / 8.0){
				posPlayer.y = startY + 64;
				busy = false;
			}
		}
		else if (sprite->animation() == LEFT_STRAIGHTFALL){
			if (time >= 1.0 / 8.0  && time < 6.0 / 8.0){
				posPlayer.y += 14. / 8.0;
			}
			if (time >= 7.9 / 8.0){
				posPlayer.y = startY + 64;
				busy = false;
			}
		}
		else if (sprite->animation() == RIGHT_ARISE){
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}
		else if (sprite->animation() == LEFT_ARISE){
			if (time >= 2.9 / 8.0){
				busy = false;
			}
		}
		else if (sprite->animation() == RIGHT_VOLTERETA){

			bool colision = map->collisionMoveRight(posPlayer, glm::vec2(32, 32));
			if (colision){
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.y = startY;
				busy = false;
			}
			else{
				if (time >= 2.0 / 8.0 && time <= 5.0 / 8.0){
					posPlayer.x += 2.0*(6.0 / 8.0);
					posPlayer.y -= 2.0*(3.0 / 8.0);
				}
				if (time >= 5.0 / 8.0){
					posPlayer.x += 2.0*(6.0 / 8.0);
					posPlayer.y += 2.0*(3.0 / 8.0);
				}
				if (time >= 8.5 / 8.0){
					posPlayer.y = startY;
					busy = false;
				}
			}
		}
		else if (sprite->animation() == LEFT_VOLTERETA){
			bool colision = map->collisionMoveLeft(posPlayer, glm::vec2(32, 32));
			if (colision){
				sprite->changeAnimation(STAND_LEFT);
				posPlayer.y = startY;
				busy = false;
			}
			else{
				if (time >= 2.0 / 8.0 && time <= 5.0 / 8.0){
					posPlayer.x -= 2.0*(6.0 / 8.0);
					posPlayer.y -= 2.0*(3.0 / 8.0);
				}
				if (time >= 5.0 / 8.0){
					posPlayer.x -= 2.0*(6.0 / 8.0);
					posPlayer.y += 2.0*(3.0 / 8.0);
				}
				if (time >= 8.5 / 8.0){
					posPlayer.y = startY;
					busy = false;
				}
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
	}

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
	}*/

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

glm::fvec2 Player::getPosPlayer() {
	return posPlayer;
}

void Player::setSpikes(vector<Spikes *> &vec)
{
	spikeTraps = vec;
}

void Player::setSoldiers(vector<Soldier *> &vec)
{
	soldiers = vec;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setPortagran(Portagran* port)
{
	portagran = port;
}

void Player::setSoundManager(SoundManager2* man){
	manager = man;
}

void Player::takeDamage()
{
	if (Game::instance().getKey('a') || !alive){
		//cheat: si es manté apretat A el príncep és invulnerable
	}
	else{
		manager->playPrinceOomp();
		hp -= 1;
		//cout << "el príncep ha rebut mal\npunts de vida restants= " << hp << "\n";
	}
}

void Player::getSliced(){
	if (Game::instance().getKey('a')){
		//cheat: si es manté apretat A el príncep és invulnerable
	}
	else{
		hp -= 100;
		busy = false;
	}
}

int Player::getHp() {
	if (hp <= 0 || !alive){
		return 0;
	}
	else{
		return hp;
	}
}


