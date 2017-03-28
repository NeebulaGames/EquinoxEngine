﻿#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <assimp/scene.h>
#include "Primitive.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "Quadtree.h"

class Level : public Primitive
{
public:
	Level();
	~Level();

	GameObject* root = nullptr;
	Quadtree* quadtree = nullptr;

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	void Load(const char* path, const char* file);
	bool CleanUp() override;
	void Draw() override;

	void DrawUI();

	//unsigned GetNumMeshes() const { return meshes.size(); }
	//unsigned GetNumMaterials() const { return materials.size(); }

	//Mesh* GetMesh(unsigned index) { return meshes[index]; }
	//const Mesh* GetMesh(unsigned index) const { return meshes[index]; }

	//Material* GetMaterial(unsigned index) { return materials[index]; }
	//const Material* GetMaterial(unsigned index) const { return materials[index]; }

	GameObject* GetRootNode() { return root; }
	const GameObject* GetRootNode() const { return root; }

	GameObject* FindGameObject(const char* name);
	void LinkGameObject(GameObject* node, GameObject* destination);

private:
	void loadNodes(aiNode* originalNode, GameObject* node);

	void loadMeshes(const aiScene* scene, const char* path);

	void drawNode(GameObject* node);
	void drawHierachy(GameObject* node);

	void cleanUpNodes(GameObject* node);
};

#endif // __LEVEL_H__