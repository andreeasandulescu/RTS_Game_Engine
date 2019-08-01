#include <MapSquare.h>
#include <Drawable.h>
#include <vector>

#ifndef GAME_MAP_H
#define GAME_MAP_H

// should represent the current gameplay map

class GameMap : Drawable{
public:
	unsigned int width;
	unsigned int height;
	std::vector<Vertex> vertices;

	// GAME MAP:
	MapSquare *** map;

	// init map with even tiles:
	void InitEven(float altitude);

	virtual void UpdateMesh();
	virtual void Draw(const glm::mat4& transform);

	// init with a default width and height:
	GameMap();
	
	// init with width * height NULL tiles:
	GameMap(unsigned int width, unsigned int height);

	// free memory
	~GameMap();

};

#endif // GAME_MAP_H
