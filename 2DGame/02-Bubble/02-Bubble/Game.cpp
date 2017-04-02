#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	level = 0;
	zero = true;
	fst = true;
	scnd = true;
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	menuscene.init();
}

bool Game::update(int deltaTime)
{
	if (level == 0 && menuscene.levelFinished() && zero) changeLevel();
	else if (level == 1 && scene.levelFinished() && fst) changeLevel();
	else if (level == 2 && scene2.levelFinished() && scnd) changeLevel();

	if (level==1) scene.update(deltaTime);
	else if (level == 2) scene2.update(deltaTime);
	else if (level == 3) scene3.update(deltaTime);
	else if (level == 0) menuscene.update(deltaTime);
	
	return bPlay;
}

void Game::changeLevel(){
	if (level == 0){
		menuscene.stop();
		menuscene.~MainMenuScene();
		zero = false;
		scene.init();
	}
	else if (level == 1){
		scene.stop();
		scene.~Scene();
		fst = false;
		scene2.init();
	}
	else if (level == 2){
		scene2.stop();
		scene2.~Scene2();
		scnd = false;
		scene3.init();
	}
	level++;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (level == 1) scene.render();
	else if (level == 2) scene2.render();
	else if (level == 3) scene3.render();
	else if (level == 0) menuscene.render();
}


void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}


bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





