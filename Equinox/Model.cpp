#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include <GL/glew.h>
#include <IL/ilut.h>
#include "ModuleTextures.h"
#include "Engine.h"

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

	_vertexIDs = new GLuint[scene->mNumMeshes];
	_normalIDs = new GLuint[scene->mNumMeshes];
	_textureIDs = new GLuint[scene->mNumMeshes];
	_indexesID = new GLuint[scene->mNumMeshes];

	glGenBuffers(scene->mNumMeshes, _vertexIDs);
	glGenBuffers(scene->mNumMeshes, _normalIDs);
	glGenBuffers(scene->mNumMeshes, _textureIDs);
	glGenBuffers(scene->mNumMeshes, _indexesID);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		GLuint* indexes = new Uint32[mesh->mNumFaces * 3];

		for (unsigned iFace = 0; iFace < mesh->mNumFaces; ++iFace)
		{
			aiFace* face = &mesh->mFaces[iFace];
			
			indexes[(iFace * 3)] = face->mIndices[0];
			indexes[(iFace * 3) + 1] = face->mIndices[1];
			indexes[(iFace * 3) + 2] = face->mIndices[2];
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vertexIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->mNumVertices * 3, &mesh->mVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _normalIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->mNumVertices * 3, &mesh->mNormals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _textureIDs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->mNumVertices * 3, &mesh->mTextureCoords[0][0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesID[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, indexes, GL_STATIC_DRAW);

		RELEASE_ARRAY(indexes);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	LOG("Loaded %i meshes and %i materials", scene->mNumMeshes, scene->mNumMaterials);
}

bool Model::CleanUp()
{
	RELEASE_ARRAY(materials);

	glDeleteBuffers(scene->mNumMeshes, _vertexIDs);
	glDeleteBuffers(scene->mNumMeshes, _normalIDs);
	glDeleteBuffers(scene->mNumMeshes, _textureIDs);
	glDeleteBuffers(scene->mNumMeshes, _indexesID);

	RELEASE_ARRAY(_vertexIDs);
	RELEASE_ARRAY(_normalIDs);
	RELEASE_ARRAY(_textureIDs);
	RELEASE_ARRAY(_indexesID);

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
		
		glBindBuffer(GL_ARRAY_BUFFER, _vertexIDs[i]);
		glVertexPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, _normalIDs[i]);
		glNormalPointer(GL_FLOAT, 0, nullptr);
		
		glBindBuffer(GL_ARRAY_BUFFER, _textureIDs[i]);
		glTexCoordPointer(3, GL_FLOAT, 0, nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesID[i]);
		glDrawElements(GL_TRIANGLES, mesh->mNumFaces * 3, GL_UNSIGNED_INT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopMatrix();
}
