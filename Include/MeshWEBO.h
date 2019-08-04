#include <Mesh.h>

#ifndef MESHWEBO_H
#define MESHWEBO_H

//class with EBO
class MeshWEBO: public Mesh
{
public:
	std::vector<unsigned int> indices;
	unsigned int EBO;

	//for MeshWEBO class
	virtual void InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<unsigned int>& indices, const Shader& shader);
	virtual void InitMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader);

	virtual Mesh& operator=(const Mesh& m);
	virtual void UpdateMesh();
	virtual void Draw(GLenum mode);
	virtual void Cleanup();

private:
	virtual void createNewMesh();

};
#endif	// MESHWEBO_H