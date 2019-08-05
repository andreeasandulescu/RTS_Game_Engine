#version 460 core

out vec4 FragColor;

in fData
{
    vec3 normal;
    vec4 color;
}frag;

uniform mat4 transform;
uniform float time;

void main()
{
	FragColor = vec4(frag.normal, 1.0f);
}