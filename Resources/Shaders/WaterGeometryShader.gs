#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out fData
{
    vec3 position;
    vec3 normal;
    vec4 color;
}frag; 

uniform mat4 transform;
uniform float time;

void main() {

    // compute normal:
    vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 v2 = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
    vec3 normal = normalize(cross(v2, v1));
    float transparency =0.97f;
    vec3 ocean_color = vec3(70.0f, 110.0f, 140.0f) / 255.0f;

    gl_Position = transform * gl_in[0].gl_Position;
    frag.color = vec4(ocean_color, transparency);
    frag.position = gl_in[0].gl_Position.xyz;
    frag.normal = normal;
    EmitVertex();

    gl_Position = transform * gl_in[1].gl_Position;
    frag.color = vec4(ocean_color, transparency);
    frag.position = gl_in[1].gl_Position.xyz;
    frag.normal = normal;
    EmitVertex();

    gl_Position = transform * gl_in[2].gl_Position;
    frag.color = vec4(ocean_color, transparency);
    frag.position = gl_in[2].gl_Position.xyz;
    frag.normal = normal;
    EmitVertex();

    EndPrimitive();
}