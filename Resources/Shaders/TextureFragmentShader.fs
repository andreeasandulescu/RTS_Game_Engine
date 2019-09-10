#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	vec4 texColor = texture(ourTexture, TexCoord);
	FragColor = texColor;
}