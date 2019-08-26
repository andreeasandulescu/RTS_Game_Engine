#include <Drawable.h>
#include <GUI.h>
#include <Message.h>
#include <Cursor.h>

#ifndef BUTTON_UI_H
#define BUTTON_UI_H

/*
	Rectangle figured button.
	Can be clicked to change the scene/produce an event.

*/
class ButtonUI : public Drawable {
public:
	// pixel values:
	unsigned int width;
	unsigned int height;

	// bottom left corner coordinates of rectangle
	unsigned int xPos; // width
	unsigned int yPos; // height

	// for scaling we need the container window:
	GLFWwindow* window;

	// text drawing methods:
	GUI* gui;

	// text inside button, text color, text scale:
	unsigned int xTextPos;
	unsigned int yTextPos;

	std::string text;
	glm::vec3 textColor;
	float textScale;

	// set text inside the button
	void setText(std::string text, glm::vec3 textColor, float textScale);

	// message sent on button click:
	ButtonPressed* m;
	bool buttonClicked(Cursor c);
	ButtonAction action;
	void* nextScene;

	void UpdateMesh();
	void Draw(const glm::mat4& transform);
	void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);
	
	ButtonUI(GLFWwindow* window, ButtonAction action, unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos);
};

#endif // BUTTON_UI_H