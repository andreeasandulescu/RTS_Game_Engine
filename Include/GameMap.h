#include <MapSquare.h>
#include <vector>

#ifndef GAME_MAP_H
#define GAME_MAP_H

// should represent the current gameplay map

class GameMap {
	unsigned int width;
	unsigned int height;

	// GAME MAP:
	MapSquare ** map;

	GameMap();
};

#endif // GAME_MAP_H
