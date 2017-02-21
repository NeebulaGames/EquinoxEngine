#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include <GL/glew.h>
#include <IL/ilut.h>
#include "ModuleTextures.h"

typedef std::vector<std::vector<int>> VII;

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char* path, const char* file)
{
	LOG("Loading model %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	scene = aiImportFile(filePath, aiProcess_PreTransformVertices | aiProcess_FlipUVs);

	indexes = new Uint32*[scene->mNumMeshes];

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		indexes[i] = new Uint32[mesh->mNumFaces * 3];

		for (unsigned iFace = 0; iFace < mesh->mNumFaces; ++iFace)
		{
			aiFace* face = &mesh->mFaces[iFace];
			
			indexes[i][(iFace * 3)] = face->mIndices[0];
			indexes[i][(iFace * 3) + 1] = face->mIndices[1];
			indexes[i][(iFace * 3) + 2] = face->mIndices[2];
		}
	}

	materials = new GLuint[scene->mNumMaterials];
	
	for(int i = 0; i < scene->mNumMaterials; ++i)
	{
		int numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTexturesByMaterial > 0)
		{
			aiMaterial* material = scene->mMaterials[i];

			aiString fileName;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
			
			sprintf_s(filePath, "%s%s", path, fileName.C_Str());

			GLuint matNum = App->textures->Load(filePath);
			assert(matNum != 0);

			materials[i] = matNum;
		}
		else
			materials[i] = 0;
	}

	LOG("Loaded %i meshes and %i materials", scene->mNumMeshes, scene->mNumMaterials);
}

bool Model::CleanUp()
{
	for (int i = 0; i < scene->mNumMeshes; ++i)
		RELEASE_ARRAY(indexes[i]);
	RELEASE_ARRAY(indexes);
	RELEASE_ARRAY(materials);

	return true;
}

void Model::Draw()
{
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		
		glBindTexture(GL_TEXTURE_2D, materials[mesh->mMaterialIndex]);

		glVertexPointer(3, GL_FLOAT, sizeof(aiVector3D), &mesh->mVertices[0]);
		glNormalPointer(GL_FLOAT, sizeof(aiVector3D), &mesh->mNormals[0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector3D), &mesh->mTextureCoords[0][0]);
		glDrawElements(GL_TRIANGLES, mesh->mNumFaces * 3, GL_UNSIGNED_INT, indexes[i]);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}
