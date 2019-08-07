#version 460 core

const int MAX_JOINTS = 20;

layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 weights;
layout (location = 4) in ivec3 jointIds;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 transform;

out vec2 TexCoord;

void main()
{
	vec4 newPosition = vec4(0.0);

	for(int i=0; i < 3; i++)
	{
		if(weights[i] > 0.1f)
		{
			mat4 jointM = jointTransforms[jointIds[i]];
			newPosition += weights[i] * jointM * vec4(inPos, 1.0f);
		}
		
	}

	gl_Position = transform * newPosition;
	TexCoord = aTexCoord;
}