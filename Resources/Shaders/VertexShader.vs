#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 color;

uniform mat4 transform;

out vec4 vertexColor;

void main()
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = color;
}


