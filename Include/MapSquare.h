#include <glm/gtc/type_ptr.hpp>
#include <Mesh.h>

#ifndef MAP_SQUARE_H
#define MAP_SQUARE_H

// GFL - GLM FOR LIFE
/*

 V1        V0
  +--------+
  |        |
  |        |
  |        |
  |        |
  +--------+
V2         V3

*/

class MapSquare {
public:
	// square points cooridnates:
	// trigonometric from top right:
	Vertex v0;
	Vertex v1;
	Vertex v2;
	Vertex v3;

	// set same Y too all map square vertices
	void SetAltitude(float h);

	// update vertex normals:
	void updateNormals();

	// update x and z coordinates of map square
	void updateCoordinates(unsigned int i, unsigned int j);

	MapSquare();
};

#endif // MAP_SQUARE_H
