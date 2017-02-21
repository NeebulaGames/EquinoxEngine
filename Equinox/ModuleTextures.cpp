#include "Globals.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

#include "SDL_image/include/SDL_image.h"
#include <IL/ilut.h>
#include <cassert>
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

using namespace std;

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Texture Manager");
	bool ret = true;

	// load support for the PNG image format
	ilInit();
	iluInit();
	ilutInit();

	// Enable OpenGL access to DevIL
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Texture Manager");

	for (TextureMap::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		glDeleteTextures(1, &it->second);
	}

	textures.clear();
	return true;
}

// Load new texture from file path
unsigned ModuleTextures::Load(const string& path)
{
	TextureMap::iterator it = textures.find(path);

	if (it != textures.end())
		return it->second;

	unsigned textureID = 0;

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilLoadImage(path.c_str());

	ILubyte* data = ilGetData();
	if (!data) {
		ilBindImage(0);
		ilDeleteImages(1, &imageID);
		return 0;
	}

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	int components = 3;
	int format = GL_RGB;
	switch (ilGetInteger(IL_IMAGE_FORMAT))
	{
	case IL_RGB:
		components = 3;
		format = GL_RGB;
		break;
	case IL_RGBA:
		components = 4;
		format = GL_RGBA;
		break;
	case IL_BGR:
		components = 3;
		format = GL_BGR_EXT;
		break;
	case IL_BGRA:
		components = 4;
		format = GL_BGRA_EXT;
		break;
	default:
		assert(false);
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImage(imageID);

	textures[path] = textureID;

	return textureID;
}

// Free texture from memory
void ModuleTextures::Unload(unsigned id)
{
	for (TextureMap::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (it->second == id)
		{
			glDeleteTextures(1, &it->second);
			textures.erase(it);
			break;
		}
	}
}