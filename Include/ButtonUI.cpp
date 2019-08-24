#include <ButtonUI.h>

void ButtonUI::UpdateMesh() {
	// create a screen button:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.resize(4);

	vertices[0].position = glm::vec3(xPos, yPos, 0.0f);
	vertices[1].position = glm::vec3(xPos + width, yPos, 0.0f);
	vertices[2].position = glm::vec3(xPos + width, yPos + height, 0.0f);
	vertices[3].position = glm::vec3(xPos, yPos + height, 0.0f);
	
	glm::vec4 buttonColor = glm::vec4(0.25f, 0.87f, 0.81f, 0.4f);

	vertices[0].auxVars = buttonColor;
	vertices[1].auxVars = buttonColor;
	vertices[2].auxVars = buttonColor;
	vertices[3].auxVars = buttonColor;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);


	Shader shader(
		"..\\Resources\\Shaders\\VertexShader.vs",
		"..\\Resources\\Shaders\\FragmentShader.fs"
	);

	mesh.InitMeshEBO(vertices, indices, shader);
}

void ButtonUI::Draw(const glm::mat4& transform) {
	glDisable(GL_DEPTH_TEST);

	// draw quad representing the button:
	mesh.shader.use();
	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mesh.DrawEBO(GL_TRIANGLES);

	// draw button and text:
	gui->RenderText(window, gui->textShader, text, xTextPos, yTextPos, textScale, textColor);

	glEnable(GL_DEPTH_TEST);
}

void  ButtonUI::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {

}

void ButtonUI::setText(std::string text, glm::vec3 textColor, float textScale) {
	this->text = text;
	this->textColor = textColor;
	this->textScale = textScale;
}

ButtonUI::ButtonUI(GLFWwindow* window, ButtonAction action, unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos) :
	window(window), action(action), width(width), height(height), xPos(xPos), yPos(yPos)
{
	text = std::string("Unnamed button.");
	textColor = glm::vec3(1.0f, 0.0f, 1.0f);
	textScale = 1.0f;

	xTextPos = 0;
	yTextPos = 0;
}

bool ButtonUI::buttonClicked(Cursor c) {
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	c.yPos = windowHeight - c.yPos;
	if (
		(xPos < c.xPos) && 
		(c.xPos < xPos + width) && 
		(yPos < c.yPos) &&
		(c.yPos < yPos + height)
	) {
		return true;
	}

	return false;
}