#include <MapSquare.h>

#ifndef UNIT_COMMAND_H
#define UNIT_COMMAND_H

enum CommandType {MOVE, STOP};

class UnitCommand {
public:
	const MapSquare* locationSquare;
	glm::vec3 positon;

	CommandType cmdType;
	UnitCommand();
};

#endif // UNIT_COMMAND_H