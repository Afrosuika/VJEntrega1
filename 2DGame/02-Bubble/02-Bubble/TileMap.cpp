#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	//Introduim mida dels tiles - 64x64
	sstream >> tileSizeX >> tileSizeY;
	getline(fin, line);
	sstream.str(line);
	//Introduim mida dels blocks - 64x32
	sstream >> blockSizeX >> blockSizeY;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	//cout << "_________________________________" << endl;
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == '0')
				map[j*mapSize.x+i] = -1;
			else{
				map[j*mapSize.x + i] = tile - int('A');
				//cout << tile - int('A') << ' ';
			}
		}
		//cout << endl;
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != -1)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSizeX, minCoords.y + j * tileSizeY);
				posTile.x /= 2;
				texCoordTile[0] = glm::vec2(float((tile) % 6) / tilesheetSize.x, float((tile) / 6) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileSizeX); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileSizeX); vertices.push_back(posTile.y + tileSizeY);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileSizeX); vertices.push_back(posTile.y + tileSizeY);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + tileSizeY);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

void TileMap::dibuixaMapa(const glm::ivec2 &pos) const{
	int x0, y0;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y + 64) / blockSizeY);
	for (int y = 0; y < mapSize.y; y++){
		for (int x = 0; x < mapSize.x; x++) {
			int tile = map[y*mapSize.x + x];
			if (x == x0 && y == y0) cout << 'X';
			else if (tile == 8 || tile == 12 || tile == 14) cout << 'M';
			else if (tile == 9 || tile == 10) cout << ' ';
			else cout << '_';
		}
		cout << endl;
	}
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y;
	x = (pos.x + 64) / blockSizeX;
	y = (pos.y + 64) / blockSizeY;

	int posX = (pos.x + 64 - 20) / blockSizeX;
	int tile = map[y*mapSize.x + posX];
	if (tile == 8 || tile == 12 || tile == 14){
		return true;
	}
	return false;
}


bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y;
	x = (pos.x + 64) / blockSizeX;
	y = (pos.y + 64) / blockSizeY;

	int posX = (pos.x + 64 + 20) / blockSizeX;
	int tile = map[y*mapSize.x + posX];
	if (tile == 8 || tile == 12 || tile == 14){
		return true;
	}
	return false;
}

bool TileMap::collisionMoveDownLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y0;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y + 64) / blockSizeY);
	x1 = ((pos.x + 64 - 10) / blockSizeX);
	int tile = map[y0*mapSize.x + x1];
	if (tile == 9 || tile == 10) return true;
	return false;
}

bool TileMap::collisionMoveDownRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y0;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y + 64) / blockSizeY);
	x1 = ((pos.x + 64 + 15) / blockSizeX);
	int tile = map[y0*mapSize.x + x1];
	if (tile == 9 || tile == 10) return true;
	return false;
}

bool TileMap::collisionClimbLeft(const glm::ivec2 &pos) const
{
	int x0, x1, y0, y12;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y + 64) / blockSizeY);

	x1 = ((pos.x + 64 - 15) / blockSizeX);
	y12 = (pos.y / blockSizeY);

	int tileUp = map[y12*mapSize.x + x0];
	int tileUpLeft = map[y12*mapSize.x + x1];
	bool up = (tileUp == 9 || tileUp == 10);
	bool upleft1 = tileUpLeft != 6 && tileUpLeft != 7 && tileUpLeft != 8;
	bool upleft2 = tileUpLeft != 9 && tileUpLeft != 10 && tileUpLeft != 11 && tileUpLeft != 12 && tileUpLeft != 14;
	if (up && upleft1 && upleft2) return true;

	for (float pix = 0; pix <= 15; pix += 0.5){
		x1 = ((pos.x + 64 + pix) / blockSizeX);
		tileUp = map[y12*mapSize.x + x1];
		tileUpLeft = map[y12*mapSize.x + x0];
		bool up = (tileUp == 9 || tileUp == 10);
		bool upleft1 = tileUpLeft != 6 && tileUpLeft != 7 && tileUpLeft != 8;
		bool upleft2 = tileUpLeft != 9 && tileUpLeft != 10 && tileUpLeft != 11 && tileUpLeft != 12 && tileUpLeft != 14;
		if (up && upleft1 && upleft2) return true;
	}

	return false;
}

bool TileMap::collisionClimbRight(const glm::ivec2 &pos) const
{
	int x0, x1, y0, y12;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y + 64) / blockSizeY);

	x1 = ((pos.x + 64 + 20) / blockSizeX);
	y12 = (pos.y / blockSizeY);

	int tileUp = map[y12*mapSize.x + x0];
	int tileUpLeft = map[y12*mapSize.x + x1];

	bool up = (tileUp == 9 || tileUp == 10);
	bool upleft1 = tileUpLeft != 6 && tileUpLeft != 7 && tileUpLeft != 8;
	bool upleft2 = tileUpLeft != 9 && tileUpLeft != 10 && tileUpLeft != 11 && tileUpLeft != 12 && tileUpLeft != 14;
	if (up && upleft1 && upleft2) return true;

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos) const
{
	int x0, y0, y1;
	x0 = ((pos.x + 64) / blockSizeX);
	y0 = ((pos.y) / blockSizeY);
	y1 = ((pos.y + 64) / blockSizeY);
	int tile = map[y0*mapSize.x + x0];
	int tileDown = map[y1*mapSize.x + x0];
	cout << tile << ' ' << tileDown << endl;
	if (tileDown == 9 || tileDown == 10) return true;
	return false;
}




























