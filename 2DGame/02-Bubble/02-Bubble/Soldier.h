#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE

#include "Sprite.h"
#include <vector>
#include "Sprite.h"
#include "TileMap.h"
#include <ctime> 
#include <time.h>
#include "Spikes.h"
#include "SoundManager2.h"

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
	void setSoundManager(SoundManager2* manager);
	void setSpikes(vector<Spikes*> &vec);
	void takeDamage();
	bool isAlive();
	void getSliced();


private:
	bool fall;
	bool busy;
	bool alive;
	bool spikedanger;
	bool takingdamage;
	int hp, type, state, aggrodistance;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posSoldier;
	float startY;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	SoundManager2 *manager;
	clock_t stamp;
	vector<Spikes*> spikeTraps;
	bool dealtdamage;

};


#endif // _SOLDIER_INCLUDE