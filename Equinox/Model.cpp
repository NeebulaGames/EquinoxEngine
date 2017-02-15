#include "Model.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Globals.h"
#include <GL/glew.h>

typedef std::vector<std::vector<int>> VII;

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);

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
}

void Model::Clear()
{
	RELEASE(scene);
}

void Model::Draw()
{
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		
		glVertexPointer(3, GL_FLOAT, sizeof(aiVector3D), &mesh->mVertices[0]);
		glNormalPointer(GL_FLOAT, sizeof(aiVector3D), &mesh->mNormals[0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector2D), &mesh->mTextureCoords[0]);
		glDrawElements(GL_TRIANGLES, mesh->mNumFaces * 3, GL_UNSIGNED_INT, indexes[i]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}
