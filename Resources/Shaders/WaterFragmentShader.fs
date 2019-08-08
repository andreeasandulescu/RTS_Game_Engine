#version 460 core

out vec4 FragColor;

struct LightSource
{
        int Type;
        vec3 Position;
        vec3 Attenuation;
        vec3 Direction;
        vec3 Color;
};

uniform int nrLights;
uniform LightSource Light[4];
uniform vec3 viewerPos;

in fData
{
	vec3 position;
    vec3 normal;
    vec4 color;
}frag;

uniform mat4 transform;
uniform float time;

void main()
{
	// compute fragment color using Phong lighting:
	float ks = 0.3; // specular constant
	float kd = 0.3; // diffuse constant
	float ka = 0.3; // ambient light constant

	vec3 ia = frag.color.xyz;

	vec3 ambientComponent = ka * Light[0].Color;
	vec3 diffuseComponent =vec3(0);
	vec3 specularComponent = vec3(0);

	vec3 N = frag.normal; // normal on the surface
	vec3 L = vec3(0);     // light direction on surface
	vec3 V = normalize(viewerPos - frag.position); // viewer fragment direction
	vec3 R = vec3(0);
	float LN_dot = 0;
	vec3 lightColor = vec3(0);

	for (int i = 0; i < nrLights; i++) {
		// compute diffuse light:
		L = normalize(Light[i].Position - frag.position);
		LN_dot = dot(L, N);

		if (LN_dot > 0) {
			// fragment is touched by light:
			diffuseComponent = diffuseComponent + 
			kd * LN_dot * Light[i].Color;

			// compute specular light:
			R = normalize(2 * LN_dot * N - L);
			specularComponent = specularComponent + ks * 
				pow(abs(dot(R, V)), 10) * Light[i].Color;
		}
		
	}

	lightColor = ambientComponent + diffuseComponent + specularComponent;

	// frag.normal
	FragColor = vec4(lightColor, 1.0f) * frag.color;
}