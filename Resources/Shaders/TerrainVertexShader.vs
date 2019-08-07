#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 transform;
uniform float time;

out vec4 vertexColor;
out vec2 TexCoord;
out vec3 realPosition;
out vec3 fragNormal;

void main()
{	
	vec4 pos; 
	realPosition = aPos;
	pos = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	fragNormal = normal;
	gl_Position = pos;
	vertexColor = vec4(normal, 1.0f);
	TexCoord = texCoords;
}


