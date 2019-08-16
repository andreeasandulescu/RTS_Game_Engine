#include <GameLogic.h>


void GameLogic::receiveMessage(Message* m) {

	if (m->messageType == MessageType::worldclick) {
		WorldClick* c = (WorldClick*)m;

		// TODO: move selected units:
		for (int i = 0; i < playerUnits.size(); i++) {
			glm::vec3 newDir = c->worldPosition - playerUnits[i].position;
			playerUnits[i].direction = glm::normalize(newDir);
		}
	}

	delete m;
}

void GameLogic::update(float deltaFrame) {
	// update all untis:
	for (int i = 0; i < playerUnits.size(); i++) {
		playerUnits[i].updateUnit(deltaFrame);
	}
}

GameLogic::GameLogic() {
	// TODO: add units:
	Unit u1 = Unit();
	u1.position = glm::vec3(10, 10, 10);
	u1.speed = 2.5f;
	playerUnits.push_back(u1);

	Unit u2 = Unit();
	u2.position = glm::vec3(10, 15, 10);
	u2.speed = 3.5f;
	playerUnits.push_back(u2);

	Unit u3 = Unit();
	u3.position = glm::vec3(10, 5, 10);
	u3.speed = 6.5f;
	playerUnits.push_back(u3);
	
}