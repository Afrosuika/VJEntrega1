#ifndef _MAIN_MENU_SCENE_INCLUDE
#define _MAIN_MENU_SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Portagran.h"
#include "SoundManager2.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <ctime> 
#include <time.h>


// It is responsible for updating and render them.


class MainMenuScene
{

public:
	MainMenuScene();
	~MainMenuScene();

	void init();
	void update(int deltaTime);
	void render();
	bool levelFinished(){ return isLevelFinished; };
	void stop();

private:
	void initShaders();
	Texture texs[2];
	TexturedQuad *texQuad;
	ShaderProgram texProgram;
	SoundManager2 *manager;
	float currentTime;
	glm::mat4 projection;
	float marginLeft, marginTop;
	bool isLevelFinished;
	bool dontRender;
	bool state;
	clock_t stamp;
};


#endif // _MAIN_MENU_SCENE_INCLUDE

