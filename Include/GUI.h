#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <string.h>
#include <algorithm>
#include <ft2build.h>
#include <stdlib.h>

#include FT_FREETYPE_H  

#ifndef _GUI_H_
#define _GUI_H_

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class GUI {
public:
	FT_Library ft;
	FT_Face face;
	std::map<GLubyte, Character> bitmap_char;
	
	// store a quad for glyph rendering:
	unsigned int VAO;
	unsigned int VBO;

	// needs to be updated when a window resize takes place
	// used in shader to draw text
	glm::mat4 projection;
	Shader textShader;

	// for fps counter:
	double lastTime;
	
	void guiUpdate(GLFWwindow* window);
	void RenderText(GLFWwindow* window, Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void initGUI();
	void exitGUI();
	GUI();
	~GUI();
};

#endif
