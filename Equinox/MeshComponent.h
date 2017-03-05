#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "BaseComponent.h"
#include <GL/glew.h>
#include <assimp/vector3.h>

struct Mesh
{
	unsigned material = 0;
	GLuint vertexID = 0;
	GLuint normalID = 0;
	GLuint textureID = 0;
	aiVector3D** textureCoords;
	GLuint indexesID = 0;
	unsigned num_vertices = 0;
	unsigned num_indices = 0;
};

class MeshComponent :
	public BaseComponent
{
public:
	Mesh* Mesh;
public:
	MeshComponent();
	~MeshComponent();

	void Update() override;
	
};

#endif
