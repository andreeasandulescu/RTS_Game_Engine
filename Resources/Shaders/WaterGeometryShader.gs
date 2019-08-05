#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out fData
{
    vec3 normal;
    vec4 color;
}frag; 

uniform mat4 transform;
uniform float time;

void main() {

    // compute normal:
    vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 v2 = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    vec3 normal = normalize(cross(v1, v2));

    gl_Position = transform * gl_in[0].gl_Position;
    frag.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    frag.normal = normal;
    EmitVertex();

    gl_Position = transform * gl_in[1].gl_Position;
    frag.color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    frag.normal = normal;
    EmitVertex();

    gl_Position = transform * gl_in[2].gl_Position;
    frag.color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    frag.normal = normal;
    EmitVertex();

    EndPrimitive();
}