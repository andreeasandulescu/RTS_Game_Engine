#version 440 core

const int MAX_JOINTS = 30;

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 weights;
layout(location = 4) in uvec4 jointIds;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 transform;

out vec2 outTexCoord;
out vec3 outNormal;

void main()
{
	vec4 newPosition = vec4(0.0f);
	vec4 newNormal = vec4(0.0f);

	for(int i = 0; i < 4; i++)
	{
		if(weights[i] > 0.0f)
		{
			mat4 jointM = jointTransforms[jointIds[i]];
			newPosition += weights[i] * jointM * vec4(position, 1.0f);
			newNormal += weights[i] * jointM * vec4(normal, 1.0f);
		}
	}
	
	gl_Position = transform * newPosition;
	outTexCoord = texCoord;
	outNormal = newNormal.xyz;
}