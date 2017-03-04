#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <assimp/vector3.h>
#include <assimp/color4.h>
#include <assimp/scene.h>
#include "Primitive.h"

class Level : public Primitive
{
public:
	Level();
	~Level();

	struct Node
	{
		std::string name;
		aiVector3D position = aiVector3D(0, 0, 0);
		aiVector3D scale = aiVector3D(1, 1, 1);
		Quat rotation = Quat(0, 0, 0, 1);
		std::vector<unsigned> meshes;
		Node* parent = nullptr;
		std::vector<Node*> childs;
	};

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

	struct Material
	{
		aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		aiColor4D specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		float shininess = 0.0f;
		unsigned texture = 0;
	};

	Node* root = nullptr;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	void Load(const char* path, const char* file);
	bool CleanUp() override;
	void Draw() override;

	unsigned GetNumMeshes() const { return meshes.size(); }
	unsigned GetNumMaterials() const { return materials.size(); }

	Mesh* GetMesh(unsigned index) { return meshes[index]; }
	const Mesh* GetMesh(unsigned index) const { return meshes[index]; }

	Material* GetMaterial(unsigned index) { return materials[index]; }
	const Material* GetMaterial(unsigned index) const { return materials[index]; }

	Node* GetRootNode() { return root; }
	const Node* GetRootNode() const { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* destination);

private:
	void loadNodes(aiNode* originalNode, Node* node);

	void loadMeshes(const aiScene* scene, const char* path);

	void drawNode(const Node* node);
};

#endif // __LEVEL_H__