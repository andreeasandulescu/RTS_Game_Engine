#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelM;
uniform mat4 projectionViewM;

out vec2 TexCoord;

void main()
{
	gl_Position = projectionViewM * modelM * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}