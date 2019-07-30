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
	// square points cooridnates:
	// trigonometric from top right:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;

	MapSquare();
};

#endif // MAP_SQUARE_H
