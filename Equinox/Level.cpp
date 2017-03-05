#include "Level.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "ModuleTextures.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

Level::Level()
{
	root = new GameObject;
}

Level::~Level()
{
}

void Level::Load(const char* path, const char* file)
{
	LOG("Loading level %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	const aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate);

	aiNode* node = scene->mRootNode;

	loadMeshes(scene, path);

	loadNodes(node, root, meshes[0]);

	aiReleaseImport(scene);
}

bool Level::CleanUp()
{
	for (Material* mat : materials)
	{
		RELEASE(mat);
	}
	for (Mesh* mesh : meshes)
	{
		RELEASE(mesh);
	}

	cleanUpNodes(root);

	RELEASE(root);

	return true;
}

void Level::Draw()
{
	drawNode(root);
}

GameObject* Level::FindNode(const char* name)
{
	return nullptr;
}

void Level::LinkNode(GameObject* node, GameObject* destination)
{
}

void Level::loadNodes(aiNode* originalNode, GameObject* node, Mesh* mesh)
{
	if (originalNode == nullptr)
		return;

	GameObject* children = new GameObject;

	children->Name = originalNode->mName.C_Str();
	children->SetParent(node);
	node->AddChild(children);

	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;

	originalNode->mTransformation.Decompose(scale, rotation, position);
	TransformComponent* transform = new TransformComponent;
	transform->Position = float3(position.x, position.y, position.z);
	transform->Scale = float3(scale.x, scale.y, scale.z);
	transform->Rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

	children->AddComponent((BaseComponent*)transform);

	MaterialComponent* materialComponent = new MaterialComponent;
	materialComponent->Material = materials[mesh->material];
	materialComponent->Material->textureID = mesh->textureID;

	children->AddComponent((BaseComponent*)materialComponent);

	MeshComponent* meshComponent = new MeshComponent;
	meshComponent->Mesh = mesh;
	children->AddComponent((BaseComponent*)meshComponent);

	for (int i = 0; i < originalNode->mNumChildren; ++i)
	{
		loadNodes(originalNode->mChildren[i], children, meshes[i]);
	}
}

void Level::loadMeshes(const aiScene* scene, const char* path)
{
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		Mesh* mesh = new Mesh;
		aiMesh* aMesh = scene->mMeshes[i];

		mesh->num_vertices = aMesh->mNumVertices;
		mesh->num_indices = aMesh->mNumFaces * 3;
		
		GLuint* indexes = new Uint32[aMesh->mNumFaces * 3];

		for (unsigned iFace = 0; iFace < aMesh->mNumFaces; ++iFace)
		{
			aiFace* face = &aMesh->mFaces[iFace];

			indexes[(iFace * 3)] = face->mIndices[0];
			indexes[(iFace * 3) + 1] = face->mIndices[1];
			indexes[(iFace * 3) + 2] = face->mIndices[2];
		}

		mesh->material = aMesh->mMaterialIndex;

		if (aMesh->mVertices != nullptr)
		{
			glGenBuffers(1, &mesh->vertexID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->num_vertices * 3, &aMesh->mVertices[0], GL_STATIC_DRAW);
		}

		if (aMesh->mNormals != nullptr)
		{
			glGenBuffers(1, &mesh->normalID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->normalID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->num_vertices * 3, &aMesh->mNormals[0], GL_STATIC_DRAW);
		}
		
		if (aMesh->mTextureCoords[0] != nullptr)
		{
			glGenBuffers(1, &mesh->textureID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->textureID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->num_vertices, &aMesh->mTextureCoords[0][0], GL_STATIC_DRAW);
		}

		glGenBuffers(1, &mesh->indexesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(aiVector3D) * aMesh->mNumFaces, indexes, GL_STATIC_DRAW);

		meshes.push_back(mesh);

		RELEASE_ARRAY(indexes);
	}

	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		int numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);

		Material* material = new Material;

		if (numTexturesByMaterial > 0)
		{
			aiMaterial* aMaterial = scene->mMaterials[i];

			aiString fileName;
			aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);

			aMaterial->Get(AI_MATKEY_COLOR_AMBIENT, material->ambient);
			aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, material->diffuse);
			aMaterial->Get(AI_MATKEY_COLOR_SPECULAR, material->specular);
			aMaterial->Get(AI_MATKEY_SHININESS, material->shininess);


			sprintf_s(material->FilePath, "%s%s", path, fileName.C_Str());

			material->texture = App->textures->Load(material->FilePath);
		}
		
		materials.push_back(material);
	}
}

void Level::drawNode(GameObject* node)
{
	node->Update();

	for (GameObject* child : node->GetChilds())
	{
		drawNode(child);
	}
}

void Level::cleanUpNodes(GameObject* node)
{
	for (BaseComponent* comp : node->GetComponents())
	{
		RELEASE(comp);
	}

	for(GameObject* child : node->GetChilds())
	{
		cleanUpNodes(child);
		RELEASE(child);
	}
}
