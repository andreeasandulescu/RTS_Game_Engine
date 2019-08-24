#include <MapSquare.h>
#include <Drawable.h>
#include <Grass.h>
#include <vector>

#ifndef GAME_MAP_H
#define GAME_MAP_H

// should represent the current gameplay map

class GameMap : Drawable{
public:
	unsigned int width;
	unsigned int height;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> grassTranslVects;		//contains all the translation vectors, each one coresponding to a grass tuft
	Grass grassObject;

	// GAME MAP:
	MapSquare *** map;

	// init map with even tiles:
	void InitEven(float altitude);

	virtual void UpdateMesh();
	virtual void Draw(const glm::mat4& transform);
	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);

	// load from data a height map:
	void loadHeightMap(unsigned char* data, size_t pixel_size, int width, int height);
	
	// sum normals at square neighbours normals
	void smoothNormals();

	// set the height of a tile
	// also sets the same height for the neighbouring
	// tiles closest vertices:
	void setHeight(int i, int j, float height);

	// retrieve the nearest MapSquare
	MapSquare* getMapSquare(glm::vec3 position);

	GameMap& operator= (const GameMap& rhs);

	// init with a default width and height:
	GameMap();
	
	// init with width * height NULL tiles:
	GameMap(unsigned int width, unsigned int height);

	void GenerateGrass();
	void DrawGrass(const glm::mat4& transform);

	// free memory
	void deleteMap();
	~GameMap();

};

#endif // GAME_MAP_H
