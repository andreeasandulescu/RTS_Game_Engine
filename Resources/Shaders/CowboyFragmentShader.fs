#version 330 core

in vec2 outTexCoord;
out vec3 outNormal;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	vec4 texColor = texture(ourTexture, outTexCoord);
	FragColor = texColor;
}