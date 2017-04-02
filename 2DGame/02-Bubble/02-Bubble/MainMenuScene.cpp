#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenuScene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16 

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 6

/*#define INIT_PLAYER_X_TILES 43
#define INIT_PLAYER_Y_TILES 9*/


MainMenuScene::MainMenuScene()
{	
	manager = NULL;
}

MainMenuScene::~MainMenuScene()
{
	//cout << "voi a morir" << endl;
	if (manager != NULL)
		delete manager;
	for (int i = 0; i < 2; ++i) {
		if (texQuad[i] != NULL)
			delete texQuad[i];
	}
}


void MainMenuScene::init()
{
	initShaders();
	stamp = clock();

	manager = new SoundManager2();
	manager->playMenuMusic();

	state = true;

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1000.f, 600.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1000.f, -600.f) };
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[0].loadFromFile("images/PoP_mainmenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/PoP_instructions.png", TEXTURE_PIXEL_FORMAT_RGB);


	dontRender = false;
}

void MainMenuScene::stop(){
	dontRender = true;
}



void MainMenuScene::update(int deltaTime)
{
	float time = float(clock() - stamp) / CLOCKS_PER_SEC;
	if (!dontRender){
		currentTime += deltaTime;
		if (state){
			if (time >= 2){
				if (Game::instance().getKey('i')){
					state = false;
					stamp = clock();
				}
				else if (Game::instance().getKey('s')){
					isLevelFinished = true;
					manager->stopAll();
				}
			}
		}
		else if (!state){
			if (time >= 2){
				if (Game::instance().getKey('r')){
					state = true;
					stamp = clock();
				}
			}
		}

		
	}
}

void MainMenuScene::render()
{
	if (!dontRender){
		glm::mat4 modelview;
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

		if (state){
			texQuad[0]->render(texs[0]);
			//cout << "dibuixa amb state" << state << "\n";
		}
		else if (!state){
			texQuad[1]->render(texs[1]);
			//cout << "dibuixa amb state" << state << "\n";
		}
		
	}
}

void MainMenuScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



