#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16 

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 6

/*#define INIT_PLAYER_X_TILES 43 
#define INIT_PLAYER_Y_TILES 9*/


Scene::Scene()
{
	map = NULL;
	player = NULL;
	manager = NULL;
}

Scene::~Scene()
{
	cout << "voi a morir" << endl;
	if(map != NULL)
		delete map;
	if (map2 != NULL)
		delete map2;
	if(player != NULL)
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
	for (int i = 0; i < portals.size(); ++i) {
		if (portals[i] != NULL)
			delete portals[i];
	}
	if (life != NULL)
		delete life;


}


void Scene::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/level02b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	/*map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/level03b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram); */
	/*map = TileMap::createTileMap("levels/Nivell1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/Nivell1b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);*/
	/*map = TileMap::createTileMap("levels/Nivell2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/Nivell2b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);*/

	manager = new SoundManager2();


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (map->getBlockSize()).x, INIT_PLAYER_Y_TILES * (map->getBlockSize()).y - 8));
	player->setTileMap(map);
	player->setSoundManager(manager);

	life = new Life();
	life->init(player, glm::vec2(0.f, 0.f), texProgram);

	
	Spikes* spiketrap = new Spikes();
	spikes.push_back(spiketrap);
	
	spikes[0]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	spikes[0]->setPosition(glm::vec2((8) * map->getTileSize(), 6 * map->getTileSize()));
	spikes[0]->setTileMap(map);
	spikes[0]->setSoundManager(manager);


	player->setSpikes(spikes);

	Soldier* asoldier = new Soldier();
	soldiers.push_back(asoldier);
	soldiers[0]->init(player,0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[0]->setPosition(glm::vec2((11) * map->getTileSize(), 6 * map->getTileSize()+8));
	soldiers[0]->setTileMap(map);
	soldiers[0]->setSpikes(spikes);
	soldiers[0]->setSoundManager(manager);

	Soldier* asoldier2 = new Soldier();
	soldiers.push_back(asoldier2);
	soldiers[1]->init(player, 1, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[1]->setPosition(glm::vec2((12.5) * map->getTileSize(), (1) * map->getTileSize() + 8));
	soldiers[1]->setTileMap(map);
	soldiers[1]->setSpikes(spikes);
	soldiers[1]->setSoundManager(manager);

	player->setSoldiers(soldiers);

	Guillotina* guillotinatrap = new Guillotina();
	guillotines.push_back(guillotinatrap);

	guillotines[0]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guillotines[0]->setPosition(glm::vec2((10) * map->getTileSize(), (6) * map->getTileSize()));
	guillotines[0]->setTileMap(map);
	guillotines[0]->setSoldiers(soldiers);
	guillotines[0]->setSoundManager(manager);


	portagran = new Portagran();
	portagran->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portagran->setPosition(glm::vec2( map->getTileSize()+20, 2 * map->getTileSize() -62));
	portagran->setTileMap(map);
	portagran->setSoundManager(manager);

	player->setPortagran(portagran);

	Portal* portal1 = new Portal();
	Portal* portal2 = new Portal();

	portal1->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal1->setPosition(glm::vec2((6) * map->getBlockSize().x, (2) * map->getTileSize()));
	portal1->setTileMap(map);
	portal1->setSoundManager(manager);
	portal1->setPair(portal2);

	portal2->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal2->setPosition(glm::vec2((8) * map->getBlockSize().x, (2) * map->getTileSize()));
	portal2->setTileMap(map);
	portal2->setSoundManager(manager);
	portal2->setPair(portal1);


	portals.push_back(portal1);
	portals.push_back(portal2);
	
	projection = glm::ortho(16.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 16.f);
	marginLeft = 16.f + 32.f;
	marginTop = 16.f + 64.f - 2.f;
	
	glm::vec4 projMargins = projectionMargins();
	projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2], projMargins[3]);
	currentTime = 0.0f;
	isLevelFinished = false;
	dontRender = false;
}

void Scene::stop(){
	dontRender = true;
}

glm::vec4 Scene::projectionMargins(){
	float left, right, top, bottom;
	glm::fvec2 pos = player->getPosPlayer();
	pos.x += 64.f;
	int quadY = (pos.y / map->getBlockSize().y) / 3;
	int quadX = ((pos.x - 16.f) / map->getBlockSize().x) /10;
	left = marginLeft + quadX*10.f*32.f;
	right = left + 10 * 32.f;
	top = marginTop + quadY*3.f*64.f;
	bottom = top + 64.f*3.f + 2.f;
	glm::vec4 ret = glm::vec4(left,right,bottom,top);
	return ret;
}

void Scene::update(int deltaTime)
{
	if (!dontRender){
		currentTime += deltaTime;
		player->update(deltaTime);
		isLevelFinished = player->levelFinished();		

		for (Spikes* spike : spikes){
			spike->update(deltaTime);
		}

		for (Portal* portal : portals){
			portal->update(deltaTime);
		}

		bool soldatsmorts = true;

		for (Soldier* soldier : soldiers){
			if (soldier->isAlive()){
				soldatsmorts = false;
			}
			soldier->update(deltaTime);
		}

		for (Guillotina* guillotina : guillotines){
			guillotina->update(deltaTime);
		}

		portagran->setMustopen(soldatsmorts);
		if (Game::instance().getKey('q')){
			portagran->setMustopen(true);
		}
		portagran->update(deltaTime);
		life->update(deltaTime);

		glm::vec4 projMargins = projectionMargins();
		projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2] + 10.f, projMargins[3] - 2.f);
		life->setPosition(glm::vec2(projMargins[0], projMargins[3]));
	}
}

void Scene::render()
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
		for (Portal* portal : portals){
			portal->render();
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

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



