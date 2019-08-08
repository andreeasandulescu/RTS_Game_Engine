#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
	// the program ID
	unsigned int id = 0;
	
	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* geometryPath,const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader();
	void Cleanup();

	Shader& operator=(const Shader& s);
	
	// use/activate the shader
	void use();

private:
	// utility function for checking shader compilation/linking errors.
	void checkCompileErrors(unsigned int shader, std::string type);

};
#endif	// SHADER_H