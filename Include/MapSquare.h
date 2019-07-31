#include <glm/gtc/type_ptr.hpp>

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
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;

	// set same Y too all map square vertices
	void SetAltitude(float h);

	// update x and z coordinates of map square
	void updateCoordinates(unsigned int i, unsigned int j);

	MapSquare();
};

#endif // MAP_SQUARE_H
