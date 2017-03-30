#include "Level.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "ModuleTextures.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "ModuleWindow.h"
#include "IMGUI/imgui.h"
#include "ModuleEditor.h"
#include "Quadtree.h"
#include "ModuleEditorCamera.h"

Level::Level()
{
	root = new GameObject;

	vec minPoint = vec(-10000, -10000, -10000);
	vec maxPoint = vec(10000, 10000, 10000);

	AABB limits = AABB(minPoint, maxPoint);

	quadtree = new Quadtree(limits);

	//quadtree->Insert(root);
}

Level::~Level()
{
}

void Level::Load(const char* path, const char* file)
{
	LOG("Loading level %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiNode* node = scene->mRootNode;

	loadMeshes(scene, path);

	loadNodes(node, root);

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
	std::vector<GameObject*> visibleObjects;
	quadtree->CollectIntersections(visibleObjects, App->editorCamera->GetCamera()->GetFrustumAABB());

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = true;

	drawNode(root);

	for (GameObject* go : visibleObjects)
		go->VisibleOnCamera = false;
}

void Level::DrawUI()
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	ImGui::SetNextWindowSize(ImVec2(300, h), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);

	if (ImGui::Begin("Hierachy", nullptr, ImGuiWindowFlags_AlwaysUseWindowPadding))
	{
		for (GameObject* node : root->GetChilds())
			drawHierachy(node);
	}
	ImGui::End();
}

GameObject* Level::FindGameObject(const char* name)
{
	return nullptr;
}

void Level::LinkGameObject(GameObject* node, GameObject* destination)
{
	destination->AddChild(node);
}

void Level::loadNodes(aiNode* originalNode, GameObject* node)
{
	if (originalNode == nullptr)
		return;

	GameObject* children = new GameObject;

	children->Name = originalNode->mName.C_Str();
	children->SetParent(node);

	aiVector3D position;
	aiVector3D scale;
	aiQuaternion rotation;

	originalNode->mTransformation.Decompose(scale, rotation, position);
	TransformComponent* transform = new TransformComponent;
	transform->Position = float3(position.x, position.y, position.z);
	transform->Scale = float3(scale.x, scale.y, scale.z);
	transform->Rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

	children->AddComponent(transform);

	std::vector<vec> vertex_boundingbox;

	if (originalNode->mMeshes != nullptr)
	{
		vertex_boundingbox.resize(originalNode->mNumMeshes * 8);
		MeshComponent* meshComponent = new MeshComponent;
		children->AddComponent(meshComponent);

		MaterialComponent* materialComponent = new MaterialComponent;
		children->AddComponent(materialComponent);

		meshComponent->MaterialComponent = materialComponent;

		for (int i = 0; i < originalNode->mNumMeshes; ++i)
		{
			Mesh* mesh = meshes[originalNode->mMeshes[i]];
			
			mesh->materialInComponent = materialComponent->AddMaterial(materials[mesh->material]);
			
			meshComponent->Meshes.push_back(mesh);

			mesh->boundingBox.GetCornerPoints(&vertex_boundingbox[i * 8]);
		}
	}

	children->BoundingBox.SetNegativeInfinity();
	if (!vertex_boundingbox.empty())
		children->BoundingBox.Enclose(reinterpret_cast<float3*>(&vertex_boundingbox[0]), originalNode->mNumMeshes * 8);

	quadtree->Insert(children);
	
	for (int i = 0; i < originalNode->mNumChildren; ++i)
	{
		loadNodes(originalNode->mChildren[i], children);
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
			glGenBuffers(1, &mesh->textureCoordsID);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->textureCoordsID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->num_vertices, &aMesh->mTextureCoords[0][0], GL_STATIC_DRAW);
		}

		glGenBuffers(1, &mesh->indexesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(aiVector3D) * aMesh->mNumFaces, indexes, GL_STATIC_DRAW);

		meshes.push_back(mesh);

		mesh->boundingBox.SetNegativeInfinity();
		mesh->boundingBox.Enclose(reinterpret_cast<float3*>(&aMesh->mVertices[0]), mesh->num_vertices);

		RELEASE_ARRAY(indexes);
	}

	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* aiMat = scene->mMaterials[i];
		Material* material = new Material;

		aiColor4D ai_property;
		float shininess;

		if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ai_property) == AI_SUCCESS)
			material->ambient = ai_property;
		if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, ai_property) == AI_SUCCESS)
			material->diffuse = ai_property;
		if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, ai_property) == AI_SUCCESS)
			material->specular = ai_property;
		if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			material->shininess = shininess;

		int numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTexturesByMaterial > 0)
		{
			aiMaterial* aMaterial = scene->mMaterials[i];

			aiString fileName;
			aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
			sprintf_s(material->FilePath, "%s%s", path, fileName.C_Str());

			material->texture = App->textures->Load(material->FilePath);
		}
		
		materials.push_back(material);
	}
}

void Level::drawNode(GameObject* node)
{
	node->Update();
	
	if (App->editor->DrawHierachy)
		node->DrawHierachy();
}

void Level::drawHierachy(GameObject* node)
{
	int flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (node->GetChilds().size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (App->editor->SelectedGameObject == node)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(node->Name.c_str(), flags))
	{
		if (ImGui::IsItemClicked(0))
		{
			App->editor->SelectedGameObject = node;
		}

		for (GameObject* child : node->GetChilds())
		{
			drawHierachy(child);
		}
		ImGui::TreePop();
	}
}

void Level::cleanUpNodes(GameObject* node)
{
	for(GameObject* child : node->GetChilds())
	{
		child->CleanUp();
		cleanUpNodes(child);
		RELEASE(child);
	}
}
