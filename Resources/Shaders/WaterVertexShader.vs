#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 3) in vec3 color;

uniform mat4 transform;
uniform float time;

out vec4 vertexColor;

void main()
{	
	vec4 pos; 
	pos = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = pos;
	vertexColor = vec4(0.0f,0.0f,1.0f, 1.0f);
}


