#include <GameMap.h>

void GameMap::InitEven(float altitude) {

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			// remove all previous tiles:
			if (this->map[i][j] != NULL) {
				delete this->map[i][j];
				this->map[i][j] = NULL;
			}

			// add new map square:
			this->map[i][j] = new MapSquare();
			this->map[i][j]->SetAltitude(altitude);
			this->map[i][j]->updateCoordinates(i, j);
		}
	}
}


void GameMap::UpdateMesh()
{
	Shader shader("..\\Resources\\Shaders\\TerrainVertexShader.vs", "..\\Resources\\Shaders\\TerrainFragmentShader.fs");
	unsigned int index = 0;

	vertices.resize(height * width * 6);
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			// set vertex data for the (i, j) square:
			map[i][j]->v0.texCoords = glm::vec2(map[i][j]->v0.position.x / height, map[i][j]->v0.position.z / width);
			map[i][j]->v1.texCoords = glm::vec2(map[i][j]->v1.position.x / height, map[i][j]->v1.position.z / width);
			map[i][j]->v2.texCoords = glm::vec2(map[i][j]->v2.position.x / height, map[i][j]->v2.position.z / width);
			map[i][j]->v3.texCoords = glm::vec2(map[i][j]->v3.position.x / height, map[i][j]->v3.position.z / width);

			// set real world positions for first triangle
			
			vertices[index++] = map[i][j]->v0;
			vertices[index++] = map[i][j]->v1;
			vertices[index++] = map[i][j]->v2;

			// set real world positions for second triangle
			vertices[index++] = map[i][j]->v0;
			vertices[index++] = map[i][j]->v2;
			vertices[index++] = map[i][j]->v3;
		}
	}

	mesh.InitMesh(vertices, shader);
	
	mesh.vertices = vertices;
	mesh.shader = shader;
	
	// load textures:

	// water texture
	Texture waterBottom{};
	waterBottom.LoadTexture("..\\Resources\\Textures\\Terrain\\under_water.jpg");
	mesh.textures.push_back(waterBottom);
	
	// sand texture
	Texture sand{};
	sand.LoadTexture("..\\Resources\\Textures\\Terrain\\sand.jpg");
	mesh.textures.push_back(sand);
	
	// grass texture
	Texture grass{};
	grass.LoadTexture("..\\Resources\\Textures\\Terrain\\grass.jpg");
	mesh.textures.push_back(grass);

	// rock texture

	mesh.UpdateMesh();
}

void GameMap::loadHeightMap(unsigned char* map_data, size_t pixel_size, int width, int height) {
	this->width = width;
	this->height = height;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setHeight(i, j, ((float) (255 - map_data[i * width * 3 + j * 3])) / 10.0f );
		}
	}
}

void GameMap::Draw(const glm::mat4& transform)
{
	char textureCName[100];

	mesh.shader.use();

	// bind textures:
	for (int i = 0; i < mesh.textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
		// activate texture channel i:
		sprintf_s(textureCName, "texture%d", i);
		glUniform1i(glGetUniformLocation(mesh.shader.id, textureCName), i);
	}

	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	// send time:
	float time = glfwGetTime();
	GLint loc = glGetUniformLocation(mesh.shader.id, "time");
	if (loc != -1)
	{
		glUniform1f(loc, time);
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.Draw(GL_TRIANGLES);
}

void GameMap::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {
	char textureCName[100];

	mesh.shader.use();

	// send time:
	float time = glfwGetTime();
	GLint loc = glGetUniformLocation(mesh.shader.id, "time");
	if (loc != -1) {
		glUniform1f(loc, time);
	}

	// send camera position:
	loc = glGetUniformLocation(mesh.shader.id, "viewerPos");
	glUniform3fv(loc, 1, value_ptr(cameraPos));

	// send number light sources:
	loc = glGetUniformLocation(mesh.shader.id, "nrLights");
	glUniform1i(loc, lightSources.size());

	// send lighting data:
	for (int i = 0; i < lightSources.size(); i++) {
		sprintf_s(textureCName, "Light[%d].Position", i);
		loc = glGetUniformLocation(mesh.shader.id, textureCName);
		glUniform3fv(loc, 1, value_ptr(lightSources[i]->sourcePosition));

		sprintf_s(textureCName, "Light[%d].Color", i);
		loc = glGetUniformLocation(mesh.shader.id, textureCName);
		glUniform3fv(loc, 1, value_ptr(lightSources[i]->color));
	}

	

	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	// Draw wireframe:
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glLineWidth(1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.Draw(GL_TRIANGLES);
}

void GameMap::setHeight(int i, int j, float height) {
	
	// raise main square by Y axis:
	// and update normals for all affected squares:
	this->map[i][j]->SetAltitude(height);
	this->map[i][j]->updateNormals();
	
	// raise two vertices neighbours:
	// North neighbour:
	if (i + 1 < this->height) {
		this->map[i + 1][j]->v2.position.y = height;
		this->map[i + 1][j]->v3.position.y = height;
		this->map[i + 1][j]->updateNormals();
	}
	// East Neighbour:
	if (j + 1 < this->width) {
		this->map[i][j + 1]->v1.position.y = height;
		this->map[i][j + 1]->v2.position.y = height;
		this->map[i][j + 1]->updateNormals();
	}
	// South neighbour:
	if (i - 1 >= 0) {
		this->map[i - 1][j]->v1.position.y = height;
		this->map[i - 1][j]->v0.position.y = height;
		this->map[i - 1][j]->updateNormals();
	}
	// West Neighbour:
	if (j - 1 >= 0) {
		this->map[i][j - 1]->v0.position.y = height;
		this->map[i][j - 1]->v3.position.y = height;
		this->map[i][j - 1]->updateNormals();
	}

	// raise diagonal neighbours:
	// NE:
	if (i + 1 < this->height && j + 1 < this->width) {
		this->map[i + 1][j + 1]->v2.position.y = height;
		this->map[i + 1][j + 1]->updateNormals();
	}
	// SE:
	if (i - 1 >= 0 && j + 1 < this->width) {
		this->map[i - 1][j + 1]->v1.position.y = height;
		this->map[i - 1][j + 1]->updateNormals();
	}
	// SW:
	if (i - 1 >= 0 && j - 1 >= 0) {
		this->map[i - 1][j - 1]->v0.position.y = height;
		this->map[i - 1][j - 1]->updateNormals();
	}
	//NW:
	if (i + 1 < this->height && j - 1 >= 0) {
		this->map[i + 1][j - 1]->v3.position.y = height;
		this->map[i + 1][j - 1]->updateNormals();
	}
}



void GameMap::smoothNormals() {
	glm::vec3 normalNE;
	glm::vec3 normalSE;
	glm::vec3 normalNW;
	glm::vec3 normalSW;
	glm::vec3 sumNormals = glm::vec3(0);
	for (int i = 1; i < this->height; i++) {
		for (int j = 1; j < this->width; j++) {
			normalNE = this->map[i][j]->v2.normal;
			normalSE = this->map[i - 1][j]->v1.normal;
			normalSW = this->map[i - 1][j - 1]->v0.normal;
			normalNW = this->map[i][j - 1]->v3.normal;
			sumNormals = glm::normalize(normalNE + normalNW + normalSE + normalSW);

			this->map[i][j]->v2.normal = sumNormals;
			this->map[i - 1][j]->v1.normal = sumNormals;
			this->map[i - 1][j - 1]->v0.normal = sumNormals;
			this->map[i][j - 1]->v3.normal = sumNormals;
		}
	}
}

GameMap::GameMap() : GameMap::GameMap(10, 10) {
}

GameMap::GameMap(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

	this->map = new MapSquare * *[height];
	for (int i = 0; i < height; i++) {
		this->map[i] = new MapSquare * [width];
		for (int j = 0; j < width; j++) {
			this->map[i][j] = NULL;
		}
	}
}

GameMap::~GameMap() {
	
	// delete all map tile entries:
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (this->map[i][j] != NULL)
				delete this->map[i][j];
		}
	}

	// delete rows:
	for (int i = 0; i < height; i++) {
		delete this->map[i];
	}

	delete this->map;
}