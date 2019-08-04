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

	// load from data a height map:
	void loadHeightMap(unsigned char* data, size_t pixel_size, int width, int height);
	
	// sum normals at square neighbours normals
	void smoothNormals();

	// set the height of a tile
	// also sets the same height for the neighbouring
	// tiles closest vertices:
	void setHeight(int i, int j, float height);


	// init with a default width and height:
	GameMap();
	
	// init with width * height NULL tiles:
	GameMap(unsigned int width, unsigned int height);

	// free memory
	~GameMap();

};

#endif // GAME_MAP_H
