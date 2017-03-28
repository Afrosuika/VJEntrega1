#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE

#include "Sprite.h"
#include <vector>
#include "Sprite.h"
#include "TileMap.h"
#include <ctime> 
#include <time.h>
#include "Spikes.h"

class Player;

class Soldier
{

public:

	void init(Player *pl, int t, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSpikes(vector<Spikes*> &vec);
	void takeDamage();
	bool isAlive();


private:
	bool fall;
	bool busy;
	bool alive;
	bool spikedanger;
	bool takingdamage;
	int hp, type, state, aggrodistance;
	glm::ivec2 tileMapDispl;
	glm::ivec2 posSoldier;
	float startY;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	clock_t stamp;
	vector<Spikes*> spikeTraps;
	bool dealtdamage;

};


#endif // _SOLDIER_INCLUDE