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


void GameMap::UpdateMesh(ResourceLoader* resourceLoader)
{
	this->resourceLoader = resourceLoader;
	grassObject.resourceLoader = resourceLoader;

	Shader shader = resourceLoader->getShader(std::string("terrain"));
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
	Texture waterBottom = resourceLoader->getTexture(std::string("under_water"));
	mesh.textures.push_back(waterBottom);
	
	// sand texture
	Texture sand = resourceLoader->getTexture(std::string("sand"));
	mesh.textures.push_back(sand);
	
	// grass texture
	Texture grass = resourceLoader->getTexture(std::string("earth"));
	mesh.textures.push_back(grass);

	mesh.UpdateMesh();
	
	// update skybox:
	skyBox.UpdateMesh(resourceLoader);
	
}

void GameMap::loadHeightMap(unsigned char* map_data, size_t pixel_size, int width, int height) {
	this->width = width;
	this->height = height;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setHeight(i, j, ((float) (255 - map_data[i * width * 3 + j * 3])) / 10.0f );
		}
	}

	double timeBefore = glfwGetTime();
	GenerateGrass();
	double timeAfter = glfwGetTime();

	std::cout << "time = " << timeAfter - timeBefore << std::endl;

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

	// draw terrain surface
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.Draw(GL_TRIANGLES);

	// draw terrain vegetation:
	DrawGrass(transform);

	
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

	// draw terrain surface
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.Draw(GL_TRIANGLES);

	// draw vegetation
	DrawGrass(transform);

	// draw sky
	skyBox.Draw(transform);
}

MapSquare* GameMap::getMapSquare(glm::vec3 position) {
	int x = floor(position.x); // height
	int z = floor(position.z); // width

	if (x >= 0 && z >= 0 && x < height && z < width) {
		return this->map[x][z];
	}

	return NULL;
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

GameMap& GameMap::operator=(const GameMap& rhs) {
	this->height = rhs.height;
	this->width = rhs.width;
	this->map = rhs.map;
	this->mesh = rhs.mesh;
	this->vertices = rhs.vertices;
	
	return *this;
}

GameMap::GameMap() : GameMap::GameMap(10, 10) {
}

GameMap::GameMap(unsigned int width, unsigned int height)
{
	

	this->map = new MapSquare * *[height];
	if (map == NULL) {
		printf("Could not allocate map!.");
	}

	for (int i = 0; i < height; i++) {
		this->map[i] = new MapSquare * [width];
		for (int j = 0; j < width; j++) {
			this->map[i][j] = NULL;
		}
	}

	this->width = width;
	this->height = height;
	
}

void GameMap::GenerateGrass()
{

	for(unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++)
		{
			MapSquare* currMapSquare = map[i][j];
			if (currMapSquare->v0.position.y > 3.0f)		//if this condition is met, that map square contains grass
			{
				int cnt = 5;

				// check if square map is steep:
				if (
					glm::distance(currMapSquare->v1.normal, glm::vec3(0.0f, 1.0f, 0.0f)) < 0.3 &&
					glm::distance(currMapSquare->v3.normal, glm::vec3(0.0f, 1.0f, 0.0f)) < 0.3)
				{

					for (int k = 0; k < cnt; k++)
					{
						float r_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);		//get random number between 0.0 and 1.0
						float r_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

						currMapSquare->grassTufts.push_back(glm::vec2(r_x, r_z));

						glm::vec3 mapSquarePosition = currMapSquare->v2.position;
						glm::vec3 grassTuftPosition = glm::vec3(r_x, 0.5f, r_z) + mapSquarePosition;

						grassTranslVects.push_back(grassTuftPosition);
					}
				}
			}
		}

	grassObject.Init(grassTranslVects);
}

void GameMap::DrawGrass(const glm::mat4& transform)
{
	grassObject.Draw(transform);
}

void GameMap::deleteMap() {
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

GameMap::~GameMap() {
	
}