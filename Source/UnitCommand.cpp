#include <UnitCommand.h>

UnitCommand::UnitCommand() {
	locationSquare = NULL;
	positon = glm::vec3(0);
	cmdType = CommandType::STOP;
}