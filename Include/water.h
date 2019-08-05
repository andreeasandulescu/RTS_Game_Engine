#include <Drawable.h>

#ifndef WATER_H
#define WATER_H

class Water : public Drawable {
public:
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<Vertex>> verticesMatrix;
	std::vector<unsigned int> indices;

	// create vertices matrix:
	// populate inidices vector to define triangles
	void initWater(int width, int height, float scale, float altitude);

	// create a mesh for renderer to draw:
	virtual void UpdateMesh();

	// call every frame:
	virtual void Draw(const glm::mat4& transform);
};


#endif // WATER_H