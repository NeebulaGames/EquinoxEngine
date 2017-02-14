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

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		indexes[i] = new Uint32[mesh->mNumFaces * 3];

		for (int iFace = 0; iFace < mesh->mNumFaces; ++iFace)
		{
			aiFace* face = &mesh->mFaces[iFace];
			for (int iVert = 0; iVert < face->mNumIndices; ++iVert)
			{
				indexes[i][(iFace * 3) + iVert] = face->mIndices[iVert];
			}
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
	glBegin(GL_TRIANGLES);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (int i = 0; i < 1; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		
		//glVertexPointer(3, GL_FLOAT, 0, &mesh->mVertices[0]);
		/*glNormalPointer(GL_FLOAT, 0, &mesh->mNormals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &mesh->mTextureCoords[0]);*/
		//glDrawElements(GL_TRIANGLES, mesh->mNumFaces, GL_UNSIGNED_INT, indexes[i]);

		for (int iFace = 0; iFace < mesh->mNumFaces; ++iFace)
		{
			aiFace* face = &mesh->mFaces[iFace];
			Uint32* indexes = face->mIndices;

			glVertex3fv(&mesh->mVertices[*indexes++][0]);
			glVertex3fv(&mesh->mVertices[*indexes++][0]);
			glVertex3fv(&mesh->mVertices[*indexes][0]);
		}
	}

	glEnd();
	glPopMatrix();
}
