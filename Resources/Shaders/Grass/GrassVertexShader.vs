#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 textureCoord;
layout (location = 3) in vec3 translationV;

uniform mat4 transform;

out vec2 TexCoord;

void main()
{
	vec3 newPosition = pos + translationV; 
	gl_Position = transform * vec4(newPosition, 1.0);
	TexCoord = textureCoord;
}