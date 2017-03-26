#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include <vector>
#include "Sprite.h"
#include "TileMap.h"
#include <ctime> 
#include <time.h>
#include "Spikes.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSpikes(vector<Spikes*> &vec);
	glm::fvec2 getPosPlayer();
	
private:
	bool bJumping;
	bool busy;
	bool alive;
	bool spikedanger;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	int jumpAngle;
	float startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	clock_t stamp;
	vector<Spikes*> spikeTraps;

};


#endif // _PLAYER_INCLUDE


