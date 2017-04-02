#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene3.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16 

//#define INIT_PLAYER_X_TILES 4
//#define INIT_PLAYER_Y_TILES 6

#define INIT_PLAYER_X_TILES 19
#define INIT_PLAYER_Y_TILES 4


Scene3::Scene3()
{
	map = NULL;
	player = NULL;
	manager = NULL;
}

Scene3::~Scene3()
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


void Scene3::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/Nivell2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/Nivell2b.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	manager = new SoundManager2();

	restart();

	int width = 32.f*120.f;
	int height = 64.f*10.5f;
	projection = glm::ortho(16.f, float(width - 1), float(height - 1), 16.f);
	marginLeft = 16.f + 32.f;
	marginTop = 16.f + 64.f - 2.f;

	glm::vec4 projMargins = projectionMargins();
	//projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2], projMargins[3]);
	currentTime = 0.0f;
	isLevelFinished = false;
	dontRender = false;
}

void Scene3::restart(){
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * (map->getBlockSize()).x, INIT_PLAYER_Y_TILES * (map->getBlockSize()).y - 8));
	player->setTileMap(map);
	player->setSoundManager(manager);

	life = new Life();
	life->init(player, glm::vec2(0.f, 0.f), texProgram);


	vector<glm::vec2> posicions;
	posicions.push_back(glm::vec2(map->getBlockSize().x * 12 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 13 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 14 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 15 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 21 + 18.f, 6 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 22 + 18.f, 6 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 76 + 18.f, 2 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 82 + 18.f, 5 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 83 + 18.f, 5 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 112 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 113 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 114 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 115 + 18.f, 9 * map->getBlockSize().y));
	posicions.push_back(glm::vec2(map->getBlockSize().x * 116 + 18.f, 9 * map->getBlockSize().y));
	spikes = vector<Spikes*>();
	for (int i = 0; i < posicions.size(); ++i){
		Spikes* spiketrap = new Spikes();
		spikes.push_back(spiketrap);
		spikes[i]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		spikes[i]->setPosition(glm::vec2(posicions[i].x, posicions[i].y));
		spikes[i]->setTileMap(map);
		spikes[i]->setSoundManager(manager);
	}
	player->setSpikes(spikes);

	soldiers = vector<Soldier*>();
	Soldier* asoldier = new Soldier();
	soldiers.push_back(asoldier);
	soldiers[0]->init(player, 0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[0]->setPosition(glm::vec2(map->getBlockSize().x * 28, 4 * map->getBlockSize().y + 8));
	soldiers[0]->setTileMap(map);
	soldiers[0]->setSpikes(spikes);
	soldiers[0]->setSoundManager(manager);

	Soldier* asoldier2 = new Soldier();
	soldiers.push_back(asoldier2);
	soldiers[1]->init(player, 0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[1]->setPosition(glm::vec2(map->getBlockSize().x * 57, 2 * map->getBlockSize().y + 8));
	soldiers[1]->setTileMap(map);
	soldiers[1]->setSpikes(spikes);
	soldiers[1]->setSoundManager(manager);

	Soldier* asoldier3 = new Soldier();
	soldiers.push_back(asoldier3);
	soldiers[2]->init(player, 1, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[2]->setPosition(glm::vec2(map->getBlockSize().x * 61, 4 * map->getBlockSize().y + 8));
	soldiers[2]->setTileMap(map);
	soldiers[2]->setSpikes(spikes);
	soldiers[2]->setSoundManager(manager);

	Soldier* asoldier4 = new Soldier();
	soldiers.push_back(asoldier4);
	soldiers[3]->init(player, 1, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[3]->setPosition(glm::vec2(map->getBlockSize().x * 93, 2 * map->getBlockSize().y + 8));
	soldiers[3]->setTileMap(map);
	soldiers[3]->setSpikes(spikes);
	soldiers[3]->setSoundManager(manager);

	Soldier* asoldier5 = new Soldier();
	soldiers.push_back(asoldier5);
	soldiers[4]->init(player, 0, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	soldiers[4]->setPosition(glm::vec2(map->getBlockSize().x * 94, 4 * map->getBlockSize().y + 8));
	soldiers[4]->setTileMap(map);
	soldiers[4]->setSpikes(spikes);
	soldiers[4]->setSoundManager(manager);

	player->setSoldiers(soldiers);


	vector<glm::vec2> posicions2;
	posicions2.push_back(glm::vec2(map->getBlockSize().x * 6, 1 * map->getBlockSize().y));
	posicions2.push_back(glm::vec2(map->getBlockSize().x * 54, 4 * map->getBlockSize().y));
	posicions2.push_back(glm::vec2(map->getBlockSize().x * 102, 4 * map->getBlockSize().y));
	posicions2.push_back(glm::vec2(map->getBlockSize().x * 110, 2 * map->getBlockSize().y));
	posicions2.push_back(glm::vec2(map->getBlockSize().x * 111, 2 * map->getBlockSize().y));
	guillotines = vector<Guillotina*>();
	for (int i = 0; i < posicions2.size(); ++i){
		Guillotina* guillotinatrap = new Guillotina();
		guillotines.push_back(guillotinatrap);
		guillotines[i]->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		guillotines[i]->setPosition(glm::vec2(posicions2[i].x, posicions2[i].y));
		guillotines[i]->setTileMap(map);
		guillotines[i]->setSoldiers(soldiers);
		guillotines[i]->setSoundManager(manager);
	}

	Portal* portal1 = new Portal();
	Portal* portal2 = new Portal();

	portal1->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal1->setPosition(glm::vec2(map->getBlockSize().x * 82, 3 * map->getBlockSize().y));
	portal1->setTileMap(map);
	portal1->setSoundManager(manager);
	portal1->setPair(portal2);

	portal2->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal2->setPosition(glm::vec2(map->getBlockSize().x * 3, 1 * map->getBlockSize().y));
	portal2->setTileMap(map);
	portal2->setSoundManager(manager);
	portal2->setPair(portal1);

	Portal* portal3 = new Portal();
	Portal* portal4 = new Portal();

	portal3->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal3->setPosition(glm::vec2(map->getBlockSize().x * 6, 1 * map->getBlockSize().y));
	portal3->setTileMap(map);
	portal3->setSoundManager(manager);
	portal3->setPair(portal4);

	portal4->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	portal4->setPosition(glm::vec2(map->getBlockSize().x * 96, 3 * map->getBlockSize().y));
	portal4->setTileMap(map);
	portal4->setSoundManager(manager);
	portal4->setPair(portal3);
	portals = vector<Portal*>();
	portals.push_back(portal1);
	portals.push_back(portal2);
	portals.push_back(portal3);
	portals.push_back(portal4);

	portagran = new Portagran();
	portagran->init(player, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);//85,1
	portagran->setPosition(glm::vec2(map->getBlockSize().x * 83 + 20, 2 * map->getBlockSize().y - 62));
	portagran->setTileMap(map);
	portagran->setSoundManager(manager);

	player->setPortagran(portagran);
}

void Scene3::stop(){
	dontRender = true;
}

glm::vec4 Scene3::projectionMargins(){
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

void Scene3::update(int deltaTime)
{
	if (!dontRender){
		if (player->getHp() <= 0 && Game::instance().getKey(13)) restart();
		else{
			currentTime += deltaTime;
			player->update(deltaTime);
			isLevelFinished = player->levelFinished();

			for (Spikes* spike : spikes){
				spike->update(deltaTime);
			}

			bool soldatsmorts = true;

			for (Soldier* soldier : soldiers){
				if (soldier->isAlive()){
					soldatsmorts = false;
				}
				soldier->update(deltaTime);
			}
			portagran->setMustopen(soldatsmorts);
			if (Game::instance().getKey('q')){
				portagran->setMustopen(true);
			}

			for (Portal* portal : portals){
				portal->update(deltaTime);
			}

			for (Guillotina* guillotina : guillotines){
				guillotina->update(deltaTime);
			}

			portagran->update(deltaTime);
			life->update(deltaTime);

			glm::vec4 projMargins = projectionMargins();
			projection = glm::ortho(projMargins[0], projMargins[1], projMargins[2] + 10.f, projMargins[3] - 2.f);
			life->setPosition(glm::vec2(projMargins[0], projMargins[3]));
		}
	}
}

void Scene3::render()
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
		for (Portal* portal : portals){
			portal->render();
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

void Scene3::initShaders()
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



