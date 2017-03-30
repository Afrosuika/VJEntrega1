#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Spikes.h"
#include "Soldier.h"
#include "Guillotina.h"
#include "Portagran.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	glm::vec4 projectionMargins();

private:
	TileMap *map, *map2, *lifesIndicator;
	Player *player;
	vector<Spikes*> spikes;
	vector<Soldier*> soldiers;
	vector<Guillotina*> guillotines;
	Portagran* portagran;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float marginLeft, marginTop;
};


#endif // _SCENE_INCLUDE

