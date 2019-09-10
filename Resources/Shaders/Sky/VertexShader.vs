#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 color;

uniform mat4 transform;

out vec3 TexCoords;
out vec4 vertexColor;

void main()
{
	TexCoords = normalize(aPos - vec3(0.5));
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = color;
}


