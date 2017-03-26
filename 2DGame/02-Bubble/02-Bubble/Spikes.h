#ifndef _SPIKES_INCLUDE
#define _SPIKES_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

class Player; //forward declaration

class Spikes
{

public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:


	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	glm::ivec2 posSpikes, spriteSize;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	clock_t stamp;

};

#endif // _SPIKES_INCLUDE