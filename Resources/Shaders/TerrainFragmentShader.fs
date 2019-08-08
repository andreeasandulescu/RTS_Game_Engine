#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 realPosition;
in vec3 fragNormal;

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

uniform sampler2D texture0; // sea bottom
uniform sampler2D texture1; // sand
uniform sampler2D texture2; // grass

void main()
{	
	vec4 color = texture(texture1, TexCoord);
	
	if (realPosition.y < 3.0f) {
		color = texture(texture1, TexCoord);
	} 

	if (realPosition.y >= 3.0f && realPosition.y <= 6.0f) {
		color =  mix(texture(texture1, TexCoord), texture(texture2, TexCoord), (realPosition.y - 3.0f) / 3.0f );
	}

	if (realPosition.y > 6.0f) {
		color = texture(texture2, TexCoord);
	}

	// determine lighting:
	// compute fragment color using Phong lighting:
	float ks = 0.3; // specular constant
	float kd = 0.3; // diffuse constant
	float ka = 0.3; // ambient light constant

	vec3 ia = color.xyz;

	vec3 ambientComponent = ka * Light[0].Color;
	vec3 diffuseComponent = vec3(0);
	vec3 specularComponent = vec3(0);

	vec3 N = fragNormal; // normal on the surface
	vec3 L = vec3(0);     // light direction on surface
	vec3 V = normalize(viewerPos - realPosition); // viewer fragment direction
	vec3 R = vec3(0);
	float LN_dot = 0;
	vec3 lightColor = vec3(0);

	for (int i = 0; i < nrLights; i++) {
		// compute diffuse light:
		L = normalize(Light[i].Position - realPosition);
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

	FragColor = vec4(vec3(color.xyz * lightColor), color.a);
}