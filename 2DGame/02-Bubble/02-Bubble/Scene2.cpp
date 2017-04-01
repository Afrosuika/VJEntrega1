#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene2.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16 

//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 6

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 6


Scene2::Scene2()
{
	map = NULL;
	player = NULL;
	manager = NULL;
}

Scene2::~Scene2()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (manager != NULL)
		delete manager;
	if (portagran != NULL)
		delete portagran;
	for (int i = 0; i < spikes.size(); ++i) {
		if (spikes[i] != NULL)
			delete spikes[i];
	}
	for (int i = 0; i < soldiers.size(); ++i) {
		if (soldiers[i] != NULL)
			delete soldiers[i];
	}
	for (int i = 0; i < guillotines.size(); ++i) {
		if (guillotines[i] != NULL)
			delete guillotines[i];
	}

}


void Scene2::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/Nivell1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/Nivell1b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	manager = new SoundManager2();

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (map->getBlockSize()).x, INIT_PLAYER_Y_TILES * (map->getBlockSize()).y - 8));
	//player->setPosition(glm::vec2(map->getTileSize() * 6, map->getBlockSize().y * 6 - 8));
	
	player->setTileMap(map);
	player->setSoundManager(manager);

	life = new Life();
	life->init(player, glm::vec2(0.f, 0.f), texProgram);

	Spikes* spiketrap = new Spikes();
	vector<glm::vec2> posicions;
	posicions.push_back(glm::vec2(map->getTileSize() * 7, (map->getBlockSize()).y * 6));
	posicions.push_back(glm::vec2(map->getTileSize() * 7, (map->getBlockSize()).y * 7));
	posicions.push_back(glm::vec2(map->getTileSize() * 9, (map->getBlockSize()).y * 9));
	posicions.push_back(glm::vec2(map->getTileSize() * 4, (map->getBlockSize()).y * 35));
	posicions.push_back(glm::vec2(map->getTileSize() * 3, (map->getBlockSize()).y * 82));
	posicions.push_back(glm::vec2(map->getTileSize() * 3, (map->getBlockSize()).y * 83));

	for (int i = 0; i < posicions.size(); ++i){
		cout << "posant spike nº " << i << endl;
		spikes.push_back(spiketrap);
		spikes[i]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		spikes[i]->setPosition(glm::vec2(posicions[i].x,posicions[i].y));
		spikes[i]->setTileMap(map);
		spikes[i]->setSoundManager(manager);
	}


	player->setSpikes(spikes);

	Soldier* asoldier = new Soldier();
	soldiers.push_back(asoldier);
	soldiers[0]->init(player, 0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 7) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize() + 8));
	soldiers[0]->setTileMap(map);
	soldiers[0]->setSpikes(spikes);
	soldiers[0]->setSoundManager(manager);

	Soldier* asoldier2 = new Soldier();
	soldiers.push_back(asoldier2);
	soldiers[1]->init(player, 1, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[1]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 8.5) * map->getTileSize(), (INIT_PLAYER_Y_TILES - 5) * map->getTileSize() + 8));
	soldiers[1]->setTileMap(map);
	soldiers[1]->setSpikes(spikes);
	soldiers[1]->setSoundManager(manager);

	player->setSoldiers(soldiers);

	Guillotina* guillotinatrap = new Guillotina();
	guillotines.push_back(guillotinatrap);

	guillotines[0]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guillotines[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 6) * map->getTileSize(), (INIT_PLAYER_Y_TILES)* map->getTileSize()));
	guillotines[0]->setTileMap(map);
	guillotines[0]->setSoldiers(soldiers);
	guillotines[0]->setSoundManager(manager);


	portagran = new Portagran();
	portagran->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portagran->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (map->getBlockSize()).x + 20, INIT_PLAYER_Y_TILES * (map->getBlockSize()).y - 62));
	portagran->setTileMap(map);
	portagran->setSoundManager(manager);

	player->setPortagran(portagran);

	int width = 32.f*75.f;
	int height = 64.f*11.5f;
	projection = glm::ortho(16.f, float(width - 1), float(height - 1), 16.f);
	marginLeft = 16.f + 32.f;
	marginTop = 16.f + 64.f - 2.f;

	glm::vec4 projMargins = projectionMargins();
	//projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2], projMargins[3]);
	currentTime = 0.0f;
	isLevelFinished = false;
	dontRender = false;
}

void Scene2::stop(){
	dontRender = true;
}

glm::vec4 Scene2::projectionMargins(){
	float left, right, top, bottom;
	glm::fvec2 pos = player->getPosPlayer();
	pos.x += 64.f;
	int quadY = (pos.y / map->getBlockSize().y) / 3;
	int quadX = ((pos.x - 16.f) / map->getBlockSize().x) / 10;
	left = marginLeft + quadX*10.f*32.f;
	right = left + 10 * 32.f;
	top = marginTop + quadY*3.f*64.f;
	bottom = top + 64.f*3.f + 2.f;
	glm::vec4 ret = glm::vec4(left, right, bottom, top);
	return ret;
}

void Scene2::update(int deltaTime)
{
	if (!dontRender){
		currentTime += deltaTime;
		player->update(deltaTime);
		isLevelFinished = player->levelFinished();

		if (Game::instance().getKey('q')){
			portagran->setMustopen(true);
		}

		for (Spikes* spike : spikes){
			spike->update(deltaTime);
		}

		for (Soldier* soldier : soldiers){
			soldier->update(deltaTime);
		}

		for (Guillotina* guillotina : guillotines){
			guillotina->update(deltaTime);
		}

		portagran->update(deltaTime);
		life->update(deltaTime);

		glm::vec4 projMargins = projectionMargins();
		//projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2] + 10.f, projMargins[3] - 2.f);
		life->setPosition(glm::vec2(projMargins[0], projMargins[3]));
	}
}

void Scene2::render()
{
	if (!dontRender){
		glm::mat4 modelview;

		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		map->render();
		for (Spikes* spike : spikes){
			spike->render();
		}
		portagran->render();
		for (Soldier* soldier : soldiers){
			soldier->render();
		}
		player->render();
		for (Guillotina* guillotina : guillotines){
			guillotina->render();
		}
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		map2->render();

		life->render();
	}
}

void Scene2::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



