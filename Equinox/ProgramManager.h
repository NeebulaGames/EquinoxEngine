#ifndef __PROGRAM_MANAGER__H__
#define __PROGRAM_MANAGER__H__

#include "Module.h"
#include <map>

struct ShaderProgram
{
	GLuint id;
	std::list<GLuint> shaders;
};

class ProgramManager : public Module
{
public:
	ProgramManager();
	~ProgramManager();

	bool Init() override;
	bool CleanUp() override;

	void CreateProgram(const std::string &name);
	void AddShaderToProgram(const std::string &name, const char* filepath, const GLenum shaderType) const;

	ShaderProgram* GetProgramByName(const std::string &name) const;
	void UseProgram(const std::string &name) const;

private:
	std::map<std::string, ShaderProgram*> programs;

	void logShaderCompiler(const GLuint shader) const;
	void logProgramLinker(const ShaderProgram* program) const;

	void compileAndAttachProgramShaders(const ShaderProgram* program) const;
};

#endif

