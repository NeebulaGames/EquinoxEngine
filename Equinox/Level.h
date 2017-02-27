#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Primitive.h"
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <assimp/color4.h>
#include <assimp/scene.h>

class Level : public Primitive
{
public:
	Level();
	~Level();

	struct Node
	{
		std::string name;
		aiVector3D position = aiVector3D(0, 0, 0);
		aiQuaternion rotation = aiQuaternion(1, 0, 0, 0);
		std::vector<unsigned> meshes;
		Node* parent = nullptr;
		std::vector<Node*> childs;
	};

	struct Mesh
	{
		unsigned material = 0;
		GLuint* _vertexIDs = nullptr;
		GLuint* _normalIDs = nullptr;
		GLuint* _textureIDs = nullptr;
		GLuint* _indexesID = nullptr;
		unsigned num_vertices = 0;
		unsigned num_indices = 0;
	};

	struct Material
	{
		aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		aiColor4D specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		float shiness = 0.0f;
		unsigned texture = 0;
	};

	Node* root = nullptr;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;

	void Load(const char* path, const char* file);
	bool CleanUp() override;
	void Draw() override;

	unsigned GetNumMeshes() const { return meshes.size(); }
	unsigned GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned index) { return meshes[index]; }
	const Mesh& GetMesh(unsigned index) const { return meshes[index]; }

	Material& GetMaterial(unsigned index) { return materials[index]; }
	const Material& GetMaterial(unsigned index) const { return materials[index]; }

	Node* GetRootNode() { return root; }
	const Node* GetRootNode() const { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* destination);

private:
	const aiScene* scene = nullptr;

	
};

#endif // __LEVEL_H__