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
	x0 = (pos.x / blockSizeX) + 1;
	y0 = (pos.y / blockSizeY) + 1;
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
	int x, y0, y1;
	x = ((pos.x+10) / blockSizeX)+1;
	y0 = (pos.y / blockSizeY)+1;
	y1 = (pos.y + size.y - 1) / blockSizeY;

	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y*mapSize.x + x];
		//dibuixaMapa(pos);
		if (tile == 8 || tile == 12 || tile == 14){
			cout << "Xocant esquerra amb tile " << (map[y*mapSize.x + x]) << endl;
			return true;
		}
	}
	
	return false;
}


bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x / blockSizeX)+1;
	y0 = (pos.y / blockSizeY) + 1;
	y1 = (pos.y + size.y - 1) / blockSizeY;

	int tile = map[y0*mapSize.x + x + 1];
	int tile2 = map[y0*mapSize.x + x + 2];
	if (tile == 8 || tile == 12 || tile == 14 || tile2 == 8 || tile2 == 12 || tile2 == 14){
		return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDownLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y0, y1;

	x0 = (pos.x / blockSizeX) + 3;
	y0 = (pos.y / blockSizeY) + 1;
	x1 = x0 - 1;
	y1 = y0 + 1;
	
	int tile1 = map[y0*mapSize.x + x1];
	int x2 = (pos.x - 45) / blockSizeX + 3;
	int tile2 = map[y0*mapSize.x + x2];
	if (tile1 == 9 || tile1 == 10 || tile2 == 9 || tile2 == 10) // 9 i 10
	{
		return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDownRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y0, y1;

	x0 = ((pos.x-13) / blockSizeX) + 2;
	y0 = (pos.y / blockSizeY) + 1;
	x1 = x0 + 1;
	y1 = y0 + 1;

	int tile1 = map[y0*mapSize.x + x1];
	int x2 = (pos.x - 45) / blockSizeX + 3;
	int tile2 = map[y0*mapSize.x + x2];
	if (tile1 == 9 || tile1 == 10 || tile2 == 9 || tile2 == 10) // 9 i 10
	{
		return true;
	}

	return false;
}






























