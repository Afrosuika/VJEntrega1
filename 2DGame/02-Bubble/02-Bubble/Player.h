#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include <vector>
#include "Sprite.h"
#include "TileMap.h"
#include <ctime> 
#include <time.h>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.
class Spikes;
class Soldier;

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void takeDamage();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSpikes(vector<Spikes*> &vec);
	void setSoldiers(vector<Soldier*> &vec);
	glm::fvec2 getPosPlayer();
	void getSliced();

private:
	bool fall;
	bool bJumping;
	bool busy;
	bool alive;
	bool spikedanger;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	int jumpAngle, hp;
	float startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	clock_t stamp;
	vector<Spikes*> spikeTraps;
	vector<Soldier*> soldiers;
	bool dealtdamage;

};


#endif // _PLAYER_INCLUDE


