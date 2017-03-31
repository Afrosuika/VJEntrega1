#ifndef _SPIKES_INCLUDE
#define _SPIKES_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager2.h"
#include <time.h>

class Player; 

class Spikes
{

public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSoundManager(SoundManager2* manager);

private:

	bool busy;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	glm::ivec2 posSpikes;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	SoundManager2 *manager;
	clock_t stamp;

};

#endif // _SPIKES_INCLUDE