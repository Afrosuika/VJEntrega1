#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "CreditsScene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16 

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 6

/*#define INIT_PLAYER_X_TILES 43
#define INIT_PLAYER_Y_TILES 9*/


CreditsScene::CreditsScene()
{
	manager = NULL;
}

CreditsScene::~CreditsScene()
{
	//cout << "voi a morir" << endl;
	if (manager != NULL)
		delete manager;
	if (texQuad != NULL)
		delete texQuad;
}


void CreditsScene::init()
{
	initShaders();
	stamp = clock();
	isLevelFinished = false;

	manager = new SoundManager2();
	manager->playMenuMusic();

	glm::vec2 geom[2] = { glm::vec2(1.f, 1.f), glm::vec2(-2000.f, -1200.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(-1000.f, 600.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[0].loadFromFile("images/PoP_credits.png", TEXTURE_PIXEL_FORMAT_RGBA);


	dontRender = false;
}

void CreditsScene::stop(){
	dontRender = true;
}



void CreditsScene::update(int deltaTime)
{
	float time = float(clock() - stamp) / CLOCKS_PER_SEC;
	if (!dontRender){
		currentTime += deltaTime;
		if (time >= 2){
			if (Game::instance().getKey('r')){
				isLevelFinished = true;
				manager->stopAll();
			}
		}
	}
}

void CreditsScene::render()
{
	if (!dontRender){
		glm::mat4 modelview;
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		texQuad->render(texs[0]);
	}
}

void CreditsScene::initShaders()
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



