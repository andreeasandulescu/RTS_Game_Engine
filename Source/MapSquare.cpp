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

 ^
 x

 V1(i + 1,0,j) V0(i + 1,0,j + 1)
  +--------+
  |        |
  |        |
  |  (i,j) |
  |        |
  +--------+
V2(i,0,j) V3(i,0,j + 1) ->z

*/

void MapSquare::SetAltitude(float h) {
	this->v0.y = h;
	this->v1.y = h;
	this->v2.y = h;
	this->v3.y = h;
}

void MapSquare::updateCoordinates(unsigned int i, unsigned int j) {
	this->v0.x = i + 1;
	this->v0.z = j + 1;

	this->v1.x = i + 1;
	this->v1.z = j;

	this->v2.x = i;
	this->v2.z = j;

	this->v3.x = i;
	this->v3.z = j + 1;

}

MapSquare::MapSquare() {
	this->v0 = glm::vec3(1.0f, 0.0f, 1.0f);
	this->v1 = glm::vec3(1.0f, 0.0f, 0.0f);
	this->v2 = glm::vec3(0.0f, 0.0f, 0.0f);
	this->v3 = glm::vec3(0.0f, 0.0f, 1.0f);
}

