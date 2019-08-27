#include<SkyBox.h>
#include<stb_image.h>



void SkyBox::UpdateMesh(ResourceLoader* resourceLoader) {
	this->resourceLoader = resourceLoader;

	// Create a cubic mesh for the  sky:
	std::vector<Vertex> v;
	std::vector<Vertex> vertices;

	// load a shader for the unit:
	Shader shader = resourceLoader->getShader(std::string("sky"));

	// Define cube positions:
	v.resize(8);

	v[0].position = glm::vec3(0, 0, 0);
	v[1].position = glm::vec3(0, 0, 1);
	v[2].position = glm::vec3(1, 0, 1);
	v[3].position = glm::vec3(1, 0, 0);
	v[4].position = glm::vec3(0, 1, 0);
	v[5].position = glm::vec3(0, 1, 1);
	v[6].position = glm::vec3(1, 1, 1);
	v[7].position = glm::vec3(1, 1, 0);

	// v1v2v3
	vertices = std::vector<Vertex>
	{
		v[0], v[1], v[2],
		v[0], v[2], v[3],
		v[0], v[1], v[5],
		v[0], v[5], v[4],
		v[1], v[2], v[6],
		v[1], v[6], v[5],
		v[3], v[2], v[6],
		v[3], v[6], v[7],
		v[0], v[3], v[7],
		v[0], v[7], v[4],
		v[5], v[6], v[4],
		v[4], v[6], v[7]
	};

	for (int i = 0; i < v.size(); i++) {
		v[i].auxVars = glm::vec4(0.5f, 0.1f, 0.1f, 0.3f);
	}

	std::vector<std::string> cubeFaces = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	// load cube textures:
	

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	char mapPath[1000];
	int width, height, nrChannels;
	for (unsigned int i = 0; i < cubeFaces.size(); i++)
	{
		sprintf_s(mapPath, 1000, "..\\Resources\\Textures\\Skybox\\%s", cubeFaces[i].c_str());
		unsigned char* data = stbi_load(mapPath, &width, &height, &nrChannels, 0);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	Texture t;
	t.id = textureID;
	mesh.textures.push_back(t);
	mesh.InitMesh(vertices, shader);
}

// draw every frame mesh
void SkyBox::Draw(const glm::mat4& transform) {
	glm::mat4 skyTransform = transform *
		glm::scale(glm::mat4(1), glm::vec3(1000)) *
		glm::translate(glm::mat4(1), glm::vec3(-0.3f, -0.7f, -0.3f))
		;
	mesh.DrawCube(skyTransform, GL_TRIANGLES);
}

void SkyBox::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {

}