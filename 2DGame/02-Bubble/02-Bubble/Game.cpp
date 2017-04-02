#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Game.h"


void Game::init()
{
	level = 0;
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	menuscene.init();
}


bool Game::update(int deltaTime)
{
	if (level == 0 && menuscene.levelFinished()) changeLevel();
	else if (level == 1 && scene.levelFinished()) changeLevel();
	else if (level == 2 && scene2.levelFinished()) changeLevel();
	else if (level == 3 && scene3.levelFinished()) changeLevel();
	else if (level == 4 && creditsscene.levelFinished()) changeLevel();

	if (level == 0) menuscene.update(deltaTime); 
	else if (level == 1) scene.update(deltaTime);
	else if (level == 2) scene2.update(deltaTime);
	else if (level == 3) scene3.update(deltaTime);
	else if (level == 4) creditsscene.update(deltaTime);
	
	return bPlay;
}

void Game::changeLevel(){
	if (level == 0){
		menuscene.stop();
		//menuscene.~MainMenuScene();
		//zero = false;
		scene.init();
		level++;
	}
	else if (level == 1){
		scene.stop();
		scene.~Scene();
		//fst = false;
		scene2.init();
		level++;
	}
	else if (level == 2){
		scene2.stop();
		scene2.~Scene2();
		//scnd = false;
		scene3.init();
		level++;
	}
	else if (level == 3){
		scene3.stop();
		scene3.~Scene3();
		//thrd = false;
		creditsscene.init();
		level++;
	}
	else if (level == 4){
		creditsscene.stop();
		//creditsscene.~CreditsScene();
		//frth = false;
		level = 0;
		menuscene.init();
	}
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (level == 0) menuscene.render(); 
	else if (level == 1) scene.render();
	else if (level == 2) scene2.render();
	else if (level == 3) scene3.render();
	else if (level == 4) creditsscene.render();
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





