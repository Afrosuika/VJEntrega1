#ifndef _SCENE2_INCLUDE
#define _SCENE2_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Spikes.h"
#include "Soldier.h"
#include "Guillotina.h"
#include "Portagran.h"
#include "SoundManager2.h"
#include "Life.h"
#include "Portal.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene2
{

public:
	Scene2();
	~Scene2();

	void init();
	void update(int deltaTime);
	void render();
	bool levelFinished(){ return isLevelFinished; };
	void stop();
	void restart();

private:
	void initShaders();
	glm::vec4 projectionMargins();

private:
	TileMap *map, *map2, *lifesIndicator;
	Player *player;
	Life *life;
	vector<Spikes*> spikes;
	vector<Soldier*> soldiers;
	vector<Guillotina*> guillotines;
	vector<Portal*> portals;
	Portagran* portagran;
	ShaderProgram texProgram;
	SoundManager2 *manager;
	float currentTime;
	glm::mat4 projection;
	float marginLeft, marginTop;
	bool isLevelFinished;
	bool dontRender;
};


#endif // _SCENE2_INCLUDE

