#include <GameLogic.h>


void GameLogic::receiveMessage(Message* m) {
	float xRand, yRand;
	glm::vec3 positionChangeDir;

	if (m->messageType == MessageType::worldclick) {
		WorldClick* c = (WorldClick*)m;

		// if left click select unit if a unit is in
		// selection radius:
		if (c->cursorState.left == ButtonStatus::PRESSED) {
			for (int i = 0; i < playerUnits.size(); i++) {
				float selectDistance = glm::distance(c->worldPosition, playerUnits[i]->position);
				if (selectDistance < playerUnits[i]->selectionRadius) {
					// unit selected:
					selectedUnits.clear();
					selectedUnits.push_back(playerUnits[i]);
				}
			}
		}

		// if right click command unit to move:
		if (c->cursorState.right == ButtonStatus::PRESSED) {
			glm::vec3 movePosition = c->worldPosition;

			for (int i = 0; i < selectedUnits.size(); i++) {
				glm::vec3 newDir = c->worldPosition - selectedUnits[i]->position;
				selectedUnits[i]->currentUnitCommand;
				selectedUnits[i]->currentUnitCommand.cmdType = CommandType::MOVE;

				// if multiple units are selected we need to assign random location for destination
				if (selectedUnits.size() > 3) {
					xRand = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5f) * sqrt(selectedUnits.size()) * 1.0f;
					yRand = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5f) * sqrt(selectedUnits.size()) * 1.0f;
					positionChangeDir = glm::vec3(xRand, 0, yRand);
					movePosition = c->worldPosition + positionChangeDir;
				}

				selectedUnits[i]->currentUnitCommand.locationSquare = gameMap.getMapSquare(movePosition);
				selectedUnits[i]->currentUnitCommand.positon = movePosition;
			}
		}
		

	}

	if (m->messageType == MessageType::keyspressed) {
		KeysMessage* keyMessage = (KeysMessage*)m;
		
		
		for (int i = 0; i < keyMessage->pressedKeys.size(); i++) {
			if (keyMessage->pressedKeys[i] == GLFW_KEY_SPACE) {
				// add all units in selected bar:
				for (int i = 0; i < playerUnits.size(); i++) {
					selectedUnits.push_back(playerUnits[i]);
				}
			}
		}
	}

	delete m;
}

void GameLogic::initGameLogic(std::string mapName, ResourceLoader* resourceLoader) {
	int width, height;
	int nrChannels;
	char mapPath[1000];

	sprintf_s(mapPath, 1000, "..\\Resources\\Textures\\%s.jpg", mapName.c_str());
	// load game map:

	unsigned char* data = stbi_load(mapPath, &width, &height, &nrChannels, 0);
	gameMap = GameMap(width, height);
	gameMap.InitEven(0.0f);
	gameMap.UpdateMesh(resourceLoader);
	gameMap.loadHeightMap(data, nrChannels, width, height);

	// load units animations:
	Shader animationShader = resourceLoader->getShader(std::string("cowboy"));
	for (int i = 0; i < playerUnits.size(); i++) {
		playerUnits[i]->position = glm::vec3(50.0f + i, 5.0f, 50.0f + i);
		playerUnits[i]->newAnimModel = resLoader->LoadAnimatedModel("..\\Resources\\Models\\cowboy.dae", animationShader);
	}

	// update normals:
	gameMap.smoothNormals();
	gameMap.UpdateMesh(resourceLoader);
}

void GameLogic::update(float deltaFrame) {
	float unitDistance = 0.6f;
	
	// update all untis:
	for (int i = 0; i < playerUnits.size(); i++) {
		MapSquare* currentSquare = gameMap.getMapSquare(playerUnits[i]->position);
		if (currentSquare != NULL) {
			glm::vec3 squareCenter = currentSquare->getSquareCenter();
			playerUnits[i]->position.y = squareCenter.y;
			playerUnits[i]->updateUnit(deltaFrame);
		}
	}

	for (int i = 0; i < playerUnits.size(); i++) {
		for (int j = 0; j < playerUnits.size(); j++) {
			if (i != j) {
				glm::vec3 unitsDistVec = playerUnits[j]->position - playerUnits[i]->position;
				if (glm::length(unitsDistVec) < 0.5f) {
					playerUnits[i]->position = playerUnits[i]->position - glm::normalize(unitsDistVec) * unitDistance;
				}
			}
		}
	}
}

GameLogic::GameLogic() {
	// add units to player's command:

	for (int i = 0; i < 20; i++) {
		Unit* u3 = new Unit();
		u3->position = glm::vec3(10, 5, 10);
		u3->speed = 6.5f;
		playerUnits.push_back(u3);
	}
	
}