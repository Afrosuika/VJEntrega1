#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 6


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/level02b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	lifesIndicator = TileMap::createTileMap("levels/vida3.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()-8));
	player->setTileMap(map);
	
	Spikes* spiketrap = new Spikes();
	spikes.push_back(spiketrap);
	
	spikes[0]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	spikes[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES+4) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	spikes[0]->setTileMap(map);


	player->setSpikes(spikes);

	Soldier* asoldier = new Soldier();
	soldiers.push_back(asoldier);
	soldiers[0]->init(player,0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 7) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()+8));
	soldiers[0]->setTileMap(map);
	soldiers[0]->setSpikes(spikes);

	Soldier* asoldier2 = new Soldier();
	soldiers.push_back(asoldier2);
	soldiers[1]->init(player, 1, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[1]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 8.5) * map->getTileSize(), (INIT_PLAYER_Y_TILES-5) * map->getTileSize() + 8));
	soldiers[1]->setTileMap(map);
	soldiers[1]->setSpikes(spikes);

	player->setSoldiers(soldiers);

	Guillotina* guillotinatrap = new Guillotina();
	guillotines.push_back(guillotinatrap);

	guillotines[0]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guillotines[0]->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 6) * map->getTileSize(), (INIT_PLAYER_Y_TILES) * map->getTileSize()));
	guillotines[0]->setTileMap(map);
	guillotines[0]->setSoldiers(soldiers);
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	
	for (Spikes* spike : spikes){
		spike->update(deltaTime);
	}

	for (Soldier* soldier : soldiers){
		soldier->update(deltaTime);
	}

	for (Guillotina* guillotina : guillotines){
		guillotina->update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	map2->render();
	lifesIndicator->render();
	for (Spikes* spike : spikes){
		spike->render();
	}
	for (Soldier* soldier : soldiers){
		soldier->render();
	}
	player->render();
	for (Guillotina* guillotina : guillotines){
		guillotina->render();
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



