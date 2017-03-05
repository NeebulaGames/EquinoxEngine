#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <assimp/scene.h>
#include "Primitive.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"

class Level : public Primitive
{
public:
	Level();
	~Level();

	GameObject* root = nullptr;

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	void Load(const char* path, const char* file);
	bool CleanUp() override;
	void Draw() override;

	//unsigned GetNumMeshes() const { return meshes.size(); }
	//unsigned GetNumMaterials() const { return materials.size(); }

	//Mesh* GetMesh(unsigned index) { return meshes[index]; }
	//const Mesh* GetMesh(unsigned index) const { return meshes[index]; }

	//Material* GetMaterial(unsigned index) { return materials[index]; }
	//const Material* GetMaterial(unsigned index) const { return materials[index]; }

	GameObject* GetRootNode() { return root; }
	const GameObject* GetRootNode() const { return root; }

	GameObject* FindNode(const char* name);
	void LinkNode(GameObject* node, GameObject* destination);

private:
	void loadNodes(aiNode* originalNode, GameObject* node, Mesh* mesh);

	void loadMeshes(const aiScene* scene, const char* path);

	void drawNode(GameObject* node);

	void cleanUpNodes(GameObject* node);
};

#endif // __LEVEL_H__