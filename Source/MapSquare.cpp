#include <MapSquare.h>

/*
 ^
 x

 V1(1,0,0) V0(1,0,1)
  +--------+
  |        |
  |        |
  |        |
  |        |
  +--------+
V2(0,0,0) V3(0,0,1) ->z

*/

MapSquare::MapSquare() {
	this->v0 = glm::vec3(1.0f, 0.0f, 1.0f);
	this->v1 = glm::vec3(1.0f, 0.0f, 0.0f);
	this->v2 = glm::vec3(0.0f, 0.0f, 0.0f);
	this->v3 = glm::vec3(0.0f, 0.0f, 1.0f);
}
