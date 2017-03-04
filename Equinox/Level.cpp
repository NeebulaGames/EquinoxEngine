#include "Level.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "ModuleTextures.h"

Level::Level()
{
	root = new Node;
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

	loadNodes(node, root);

	aiReleaseImport(scene);
}

bool Level::CleanUp()
{
	return true;
}

void Level::Draw()
{
	drawNode(root);
}

Level::Node* Level::FindNode(const char* name)
{
	return nullptr;
}

void Level::LinkNode(Node* node, Node* destination)
{
}

void Level::loadNodes(aiNode* originalNode, Node* node)
{
	if (originalNode == nullptr)
		return;

	Node* children = new Node;

	children->name = originalNode->mName.C_Str();
	children->parent = node;
	for (int i = 0; i < originalNode->mNumMeshes; ++i)
		children->meshes.push_back(originalNode->mMeshes[i]);
	aiQuaternion rotation;
	originalNode->mTransformation.Decompose(children->scale, rotation, children->position);
	children->rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	node->childs.push_back(children);

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

			char filePath[256];

			sprintf_s(filePath, "%s%s", path, fileName.C_Str());

			material->texture = App->textures->Load(filePath);
		}

		materials.push_back(material);
	}
}

void Level::drawNode(const Node* node)
{
	glPushMatrix();

	glColor3f(1.f, 1.f, 1.f);

	glTranslatef(node->position.x, node->position.y, node->position.z);
	glScalef(node->scale.x, node->scale.y, node->scale.z);
	float3 rotation = node->rotation.ToEulerXYZ();
	glRotatef(node->rotation.Angle(), rotation.x, rotation.y, rotation.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (unsigned iMesh : node->meshes)
	{
		Mesh* mesh = meshes[iMesh];
		Material* mat = materials[mesh->material];

		glBindTexture(GL_TEXTURE_2D, mat->texture);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexID);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->normalID);
		glNormalPointer(GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->textureID);
		glTexCoordPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexesID);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (Node* child : node->childs)
	{
		drawNode(child);
	}

	glPopMatrix();
}
