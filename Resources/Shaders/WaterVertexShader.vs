#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 3) in vec3 color;

uniform mat4 transform;
uniform float time;

void main()
{	
	vec4 pos;
	vec2 x0 = vec2(aPos.x, aPos.z);
	vec2 x;
	float y = aPos.y;

	float g_const = 9.81;
	float pi = 3.14;
	float wave_amplitudes[3] = float[](0.5, 0.8, 0.2);
	float wave_lengths[3] = float[](20, 15, 28);
	float phases[3] = float[](0.5, 0, 2);
	vec2 wave_directions[3] = vec2[3](vec2(0.5, 0.5), vec2(0.0, -0.2), vec2(0.0, 0.8));
	float magnitudes[3];
	float frequencies[3];
	int waves_number = 3;

	for (int i = 0; i < waves_number; i++) {
		magnitudes[i] = 2 * pi / wave_lengths[i];
	} 

	for (int i = 0; i < waves_number; i++) {
		frequencies[i] = sqrt(magnitudes[i] * g_const);
	}

	vec2 sinSums = vec2(0, 0);
	for (int i = 0; i < waves_number; i++) {
		sinSums = sinSums + (wave_directions[i] / magnitudes[i]) *
			wave_amplitudes[i] *
			sin(
				dot(wave_directions[i], x0) - frequencies[i] *
					time + phases[i]
				);
	}
	x = x0 - sinSums;

	for (int i = 0; i < waves_number; i++) {
		y = y + wave_amplitudes[i] * 
			cos(dot(wave_directions[i], x0) - frequencies[i] * time + phases[i])
			;
	}

	pos = vec4(aPos, 1.0f);
	pos.x = x.x;
	pos.z = x.y;
	pos.y = y;
	// send real coords to geometry shader:
	gl_Position = pos;
}


