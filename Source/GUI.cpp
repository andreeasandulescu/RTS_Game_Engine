#include <GUI.h>


void GUI::guiUpdate(GLFWwindow* window) {
	
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	// compute fps and draw:
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	char fps_text[100];
	lastTime = currentTime;
	int fps = 1.0 / delta;
	sprintf_s(fps_text, "FPS: %d", fps);
	RenderText(window, this->textShader, std::string(fps_text), 1, height - 30, 0.3f, glm::vec3(1.0f, 1.0f, 0.0f));
}

GUI::GUI() {
	this->bitmap_char = std::map<GLubyte, Character>();
}

void GUI::initGUI() {
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, "..\\Resources\\fonts\\arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// use shader for drawing the text:
	this->textShader = Shader("..\\Resources\\Shaders\\TextVertexShader.vs", "..\\Resources\\Shaders\\TextFragmentShader.fs");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_GlyphSlot g = face->glyph;
	FT_Set_Char_Size(
		face,    /* handle to face object           */
		0,       /* char_width in 1/64th of points  */
		16 * 64,   /* char_height in 1/64th of points */
		300,     /* horizontal device resolution    */
		300);   /* vertical device resolution      */
	
	

	for (GLubyte i = 0; i < 128; i++) {

		int error = FT_Load_Char(
			face,          /* handle to face object */
			i,   /* glyph index           */
			FT_LOAD_RENDER);  /* load flags, see below */
		if (error) {
			printf("FreeType could not load glyph_index: %d.\n", i);
		}
		face->glyph->bitmap.palette_mode;

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		/*
		printf("\n\n\n");
		for (int j = 0; j < face->glyph->bitmap.width; j++) {
			for (int k = 0; k < face->glyph->bitmap.width; k++) {
				printf("%d ", *(face->glyph->bitmap.buffer + j * face->glyph->bitmap.width + k));
			}
			printf("\n");
		}
		*/

		Character character = {
		texture,
		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		face->glyph->advance.x
		};
		this->bitmap_char.insert(std::pair<GLchar, Character>(i, character));
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GUI::~GUI() {
	
}

void GUI::exitGUI() {
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void GUI::RenderText(GLFWwindow* window, Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{

	// Activate corresponding render state	
	glUseProgram(this->textShader.id);

	// send projection matrix to shader
	// allows us to specify text in screen coordinates
	glUniformMatrix4fv(glGetUniformLocation(this->textShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// send the text color to the shader
	glUniform3f(glGetUniformLocation(this->textShader.id, "textColor"), color.x, color.y, color.z);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = bitmap_char[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(glGetUniformLocation(this->textShader.id, "text"), 0);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}