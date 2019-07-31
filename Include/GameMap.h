#include <MapSquare.h>
#include <vector>

#ifndef GAME_MAP_H
#define GAME_MAP_H

// should represent the current gameplay map

class GameMap {
public:
	unsigned int width;
	unsigned int height;

	// GAME MAP:
	MapSquare *** map;

	// init map with even tiles:
	void InitEven(float altitude);

	// init with a default width and height:
	GameMap();
	
	// init with width * height NULL tiles:
	GameMap(unsigned int width, unsigned int height);

	// free memory
	~GameMap();
};

#endif // GAME_MAP_H
