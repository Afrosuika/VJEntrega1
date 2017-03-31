#ifndef _LIFE_INCLUDE
#define _LIFE_INCLUDE
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <time.h>

class Life
{
public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	clock_t stamp;
	glm::vec2 posicioVides;
	int currentLifes;
};


#endif // _LIFE_INCLUDE