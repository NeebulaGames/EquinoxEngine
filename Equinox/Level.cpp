#include "Level.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

Level::Level()
{
}

Level::~Level()
{
}

void Level::Load(const char* path, const char* file)
{
	LOG("Loading level %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s%s", path, file);

	scene = aiImportFile(filePath, aiProcess_FlipUVs);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{

	}
}

bool Level::CleanUp()
{
}

void Level::Draw()
{
}

Level::Node* Level::FindNode(const char* name)
{
}

void Level::LinkNode(Node* node, Node* destination)
{
}
