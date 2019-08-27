#include <Drawable.h>
#include <ButtonUI.h>
#include <Message.h>

#ifndef SCENE_UI_H
#define SCENE_UI_H

class SceneUI : public Drawable {
public:
	// buttons list:
	std::vector<ButtonUI*> buttons;

	// container window
	GLFWwindow* window;

	// drawing functions:
	GUI* gui;

	Message* handleClickEvent(Cursor c);

	// update at least once before drawing
	void UpdateMesh(ResourceLoader* resourceLoader);
	void Draw(const glm::mat4& transform);
	void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);


	// centered buttons
	void addButtonAlligned(ButtonAction action, std::string text, glm::vec3 textColor, float textScale);

	SceneUI(GLFWwindow* window, GUI* gui);
};

#endif // SCENE_UI_H