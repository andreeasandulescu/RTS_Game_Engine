#include <MessageReceiver.h>
#include <MessageBus.h>
#include <Unit.h>
#include <GameMap.h>
#include <stb_image.h>

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

class GameLogic : public MessageReceiver {
public:
	std::vector<Unit *> selectedUnits;
	std::vector<Unit *> playerUnits;
	ResourceLoader* resLoader;
	GameMap gameMap;
	

	// message bus for receiveing/sending messages:
	MessageBus* messageBus;

	// update logic:
	void update(float deltaFrame);

	virtual void receiveMessage(Message* m);
	void initGameLogic(std::string mapName, ResourceLoader* resourceLoader);

	GameLogic();
};

#endif GAME_LOGIC_H
