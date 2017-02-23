#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/scene.h>
#include "Primitive.h"
#include <GL/glew.h>

class Model :
	public Primitive
{
public:
	Model();
	~Model();

	void Load(const char* path, const char* file);
	bool CleanUp() override;
	void Draw() override;

private:
	const aiScene* scene = nullptr;

	GLuint* _vertexIDs;
	GLuint* _normalIDs;
	GLuint* _textureIDs;
	GLuint* _indexesID;

	GLuint* materials;
};

#endif // __MODEL_H__