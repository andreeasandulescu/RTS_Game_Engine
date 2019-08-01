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


	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//first triangle
			vertices.push_back(map[i][j]->v0);
			vertices.push_back(map[i][j]->v1);
			vertices.push_back(map[i][j]->v2);

			//second triangle
			vertices.push_back(map[i][j]->v0);
			vertices.push_back(map[i][j]->v2);
			vertices.push_back(map[i][j]->v3);
		}
	}
}


void GameMap::UpdateMesh()
{
	Shader shader("..\\Resources\\Shaders\\VertexShader.vs", "..\\Resources\\Shaders\\FragmentShader.fs");
	unsigned int index = 0;

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			//first triangle
			vertices[index++] = map[i][j]->v0;
			vertices[index++] = map[i][j]->v1;
			vertices[index++] = map[i][j]->v2;

			//second triangle
			vertices[index++] = map[i][j]->v0;
			vertices[index++] = map[i][j]->v2;
			vertices[index++] = map[i][j]->v3;
		}
	}

	mesh.InitMesh(vertices, shader);
	

	mesh.vertices = vertices;
	mesh.shader = shader;
	
	mesh.UpdateMesh();
}


void GameMap::Draw(const glm::mat4& transform)
{
	mesh.shader.use();

	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0);

	mesh.Draw(GL_TRIANGLES);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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