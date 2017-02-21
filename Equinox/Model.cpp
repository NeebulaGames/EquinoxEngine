#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include <GL/glew.h>
#include <IL/ilut.h>

typedef std::vector<std::vector<int>> VII;

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char* path, const char* file)
{
	std::string filePath(path);
	filePath.append(file);

	scene = aiImportFile(filePath.data(), aiProcess_PreTransformVertices);

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
	int numTexturesByMaterial;
	for(int i = 0; i < scene->mNumMaterials; ++i)
	{
		numTexturesByMaterial = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		//materials[i] = new GLuint[numTexturesByMaterial];
		if (numTexturesByMaterial > 0)
		{
			aiMaterial* material = scene->mMaterials[i];

			aiString fileName;
			aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
			filePath = path;
			filePath.append(fileName.C_Str());
			LOG("PATH: %s", filePath.data());

			GLuint matNum = 0;
			char tmp[512];
			strcpy_s(tmp, filePath.c_str());

			matNum = ilutGLLoadImage(tmp);



			LOG("Texture ID: %i", matNum);
			//assert(matNum != 0);

			materials[i] = matNum;
		}
		else
			materials[i] = 0;
	}

	LOG("FI LOAD");
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int numTexturesByMaterial;
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		numTexturesByMaterial = scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//for (int y = 0; y < numTexturesByMaterial; ++y)
			glBindTexture(GL_TEXTURE_2D, materials[mesh->mMaterialIndex]);

		glVertexPointer(3, GL_FLOAT, sizeof(aiVector3D), &mesh->mVertices[0]);
		glNormalPointer(GL_FLOAT, sizeof(aiVector3D), &mesh->mNormals[0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector3D), &mesh->mTextureCoords[0]);
		glDrawElements(GL_TRIANGLES, mesh->mNumFaces * 3, GL_UNSIGNED_INT, indexes[i]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}
