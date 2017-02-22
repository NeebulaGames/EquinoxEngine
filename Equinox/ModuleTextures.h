#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include <map>

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init() override;
	bool CleanUp() override;

	unsigned Load(const std::string& path);
	void Unload(unsigned id);

private:

	typedef std::map <std::string, unsigned> TextureMap;

	TextureMap _textures;
};

#endif // __MODULETEXTURES_H__