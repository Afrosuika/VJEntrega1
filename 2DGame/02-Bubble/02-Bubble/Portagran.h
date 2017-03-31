#ifndef _PORTAGRAN_INCLUDE
#define _PORTAGRAN_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager2.h"
#include <time.h>

class Player;

class Portagran
{

public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSoundManager(SoundManager2* manager);
	void setMustopen(bool cond);
	bool getOberta();

private:

	bool busy;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	glm::ivec2 posPortagran;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	SoundManager2 *manager;
	clock_t stamp;
	bool mustopen;
	bool oberta;

};

#endif // _PORTAGRAN_INCLUDE