#ifndef _PORTAL_INCLUDE
#define _PORTAL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager2.h"
#include <time.h>

class Player;

class Portal
{

public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSoundManager(SoundManager2* manager);
	void setPair(Portal* pair);
	void pairIsUsed();

private:

	bool busy;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	glm::ivec2 posPortal;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	Portal *pair;
	SoundManager2 *manager;
	clock_t stamp;

};

#endif // _PORTAL_INCLUDE