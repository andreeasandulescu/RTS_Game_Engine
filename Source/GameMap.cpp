#include <GameMap.h>

GameMap::GameMap() : GameMap::GameMap(10, 10) {

}

GameMap::GameMap(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

	this->map = new MapSquare * *[height];
	for (int i = 0; i < height; i++) {
		this->map[i] = new MapSquare * [width];
		for (int j = 0; j < width; j++) {
			this->map[i][j] = NULL;
		}
	}
}

GameMap::~GameMap() {
	
	// delete all map tile entries:
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (this->map[i][j] != NULL)
				delete this->map[i][j];
		}
	}

	// delete rows:
	for (int i = 0; i < height; i++) {
		delete this->map[i];
	}

	delete this->map;
}