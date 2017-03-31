#ifndef _GUILLOTINA_INCLUDE
#define _GUILLOTINA_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager2.h"
#include <time.h>

class Player;

class Soldier;

class Guillotina
{

public:

	void init(Player *pl, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::ivec2 getPosRender();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setSoundManager(SoundManager2* manager);
	void setSoldiers(vector<Soldier*> &vec);

private:

	bool busy;
	glm::ivec2 tileMapDispl;
	glm::fvec2 posPlayer;
	glm::ivec2 posGuillotina;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	SoundManager2 *manager;
	clock_t stamp;
	vector<Soldier*> soldats;
	bool damagedone;

};

#endif // _GUILLOTINA_INCLUDE