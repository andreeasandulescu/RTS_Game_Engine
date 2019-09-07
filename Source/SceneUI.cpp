#include <SceneUI.h>

// add button on the border
void SceneUI::addButtonBorderAlligned(ButtonAction action, std::string text, glm::vec3 textColor, float textScale) {
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	int startWidthSpace = 30;
	int startHeightSpace = 40;
	int buttonSpacing = 30;

	int fontSize = 48 * textScale;

	int buttonWidth = text.size() * fontSize;
	int buttonHeight = fontSize * 2;

	int buttonPosY = startHeightSpace;
	int buttonPosX = startWidthSpace;
	if (buttons.size() > 0) {
		buttonPosX = buttons[buttons.size() - 1]->xPos + buttons[buttons.size() - 1]->text.size() * fontSize + 2 * buttonSpacing;
	}

	ButtonUI* buttonUI = new ButtonUI(this->window, ButtonAction::unkownAction, buttonWidth, buttonHeight, buttonPosX, buttonPosY);
	buttonUI->setText(text, textColor, textScale);
	buttonUI->gui = gui;
	buttonUI->window = window;
	buttonUI->xTextPos = buttonPosX + ((text.size() - 1) / 4 * fontSize);
	buttonUI->yTextPos = buttonPosY + fontSize / 2;

	// create button message:
	ButtonPressed* m = new ButtonPressed();
	m->action = action;
	buttonUI->m = m;
	buttonUI->m->messageString = text;

	buttons.push_back(buttonUI);
}

// allign button in the center of the screen as top down list
void SceneUI::addButtonAlligned(ButtonAction action, std::string text, glm::vec3 textColor, float textScale) {
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	int centerWidth = windowWidth / 2;
	int topDist = windowHeight / 8;
	int fontSize = 48 * textScale;

	int buttonWidth = text.size() * fontSize;
	int buttonHeight = fontSize * 2;

	int buttonPosY = windowHeight - topDist - buttonHeight * 2 * (buttons.size() + 1);
	int buttonPosX = centerWidth - (text.size() / 2 * fontSize);


	ButtonUI *buttonUI = new ButtonUI(this->window, ButtonAction::unkownAction, buttonWidth, buttonHeight, buttonPosX, buttonPosY);
	buttonUI->setText(text, textColor, textScale);
	buttonUI->gui = gui;
	buttonUI->window = window;
	buttonUI->xTextPos = buttonPosX + ((text.size() - 1) / 4 * fontSize);
	buttonUI->yTextPos = buttonPosY + fontSize / 2;
	
	// create button message:
	ButtonPressed* m = new ButtonPressed();
	m->action = action;
	buttonUI->m = m;
	buttonUI->m->messageString = text;

	buttons.push_back(buttonUI);
}

void SceneUI::UpdateMesh(ResourceLoader* resourceLoader) {
	this->resourceLoader = resourceLoader;
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->UpdateMesh(resourceLoader);
	}
}

void SceneUI::Draw(const glm::mat4& transform) {
	// draw scene and all buttons:
	for (int i = 0; i < buttons.size(); i++) {
		// orthogonal projection:
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glm::mat4 transform = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
		buttons[i]->Draw(transform);
	}
}

void  SceneUI::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {

}

SceneUI::SceneUI(GLFWwindow* window, GUI* gui) : window(window), gui(gui){

}

Message* SceneUI::handleClickEvent(Cursor c) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i]->buttonClicked(c)) {
			return buttons[i]->m->clone();
		}
	}
	return NULL;
}