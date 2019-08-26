#include <Drawable.h>
#include <GUI.h>
#include <SceneUI.h>
#include <MessageBus.h>

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class UserInterface : public Drawable, public MessageReceiver {
public:
	SceneUI* currentScene;

	// container window
	GLFWwindow* window;

	// rendering functions:
	GUI* gui;

	// communication bus
	// receive click events
	// send a message when a button is clicked
	MessageBus* messageBus;

	virtual void receiveMessage(Message* m);
	
	// starts with the main menu scene
	void initUserInterface(GLFWwindow* window, GUI* gui, MessageBus* messageBus);

	// update current scene mesh
	virtual void UpdateMesh();

	// draw current scene:
	virtual void Draw(const glm::mat4& transform);
	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);

	UserInterface();
};

#endif // USER_INTERFACE_H
