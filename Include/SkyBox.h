#include <Drawable.h>

#ifndef SKY_BOX_H
#define SKY_BOX_H

class SkyBox : public Drawable {
public:
	virtual void UpdateMesh(ResourceLoader* resourceLoader);

	// draw every frame mesh
	virtual void Draw(const glm::mat4& transform);
	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);
};

#endif