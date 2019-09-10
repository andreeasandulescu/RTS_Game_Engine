#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 color;

uniform mat4 transform;

out vec4 vertexColor; 
out vec2 TexCoord;

void main()
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;
	vertexColor = vec4(color, 1.0f);
}