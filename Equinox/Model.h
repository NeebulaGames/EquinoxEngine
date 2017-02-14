#ifndef __MODEL_H__
#define __MODEL_H__

#include "Primitive.h"
#include <assimp/scene.h>
#include <SDL/include/SDL_hints.h>

class Model :
	public Primitive
{
public:
	Model();
	~Model();

	void Load(const char* file);
	void Clear();
	void Draw() override;

private:
	const aiScene* scene = nullptr;

	Uint32** indexes;
};

#endif // __MODEL_H__