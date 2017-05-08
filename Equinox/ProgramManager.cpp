#include "Globals.h"
#include "Engine.h"
#include "ProgramManager.h"

ProgramManager::ProgramManager()
{

}

ProgramManager::~ProgramManager()
{

}

bool ProgramManager::Init()
{
	return true;
}

bool ProgramManager::CleanUp()
{
	// TODO: Clean all shaders, programs and finally ShaderProgram pointers.
	return true;
}

void ProgramManager::CreateProgram(const std::string& name)
{
	if (programs.find(name) == programs.end())
	{
		GLuint shaderProgramId = glCreateProgram();
		ShaderProgram* shaderProgram = new ShaderProgram;
		shaderProgram->id = shaderProgramId;
		shaderProgram->shaders.clear();
		programs.insert(std::pair<std::string, ShaderProgram*>(name, shaderProgram));
	}
}

void ProgramManager::AddShaderToProgram(const std::string& name, const char* filepath, const GLenum shaderType) const
{
	ShaderProgram* itShader = GetProgramByName(name);
	if (itShader != nullptr)
	{
		FILE* shaderFile = fopen(filepath, "r");
		int fileSize = 0;

		char* shaderSource;

		fseek(shaderFile, 0, SEEK_END);
		fileSize = ftell(shaderFile);
		rewind(shaderFile);

		shaderSource = new char[fileSize + 1];
		fread(shaderSource, sizeof(char), fileSize, shaderFile);
		shaderSource[fileSize] = '\0';
		fclose(shaderFile);

		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, const_cast<const GLchar**>(&shaderSource), nullptr);

		itShader->shaders.push_back(shader);

		delete[] shaderSource;
	}
}

ShaderProgram* ProgramManager::GetProgramByName(const std::string &name) const
{
	auto it = programs.find(name);
	return it != programs.end() ? it->second : nullptr;
}

void ProgramManager::UseProgram(const std::string &name) const
{
	ShaderProgram* itShader = GetProgramByName(name);
	if (itShader != nullptr)
	{
		compileAndAttachProgramShaders(itShader);

		GLint isLinked = 0;
		glGetProgramiv(itShader->id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			logProgramLinker(itShader);
		}

		glUseProgram(GetProgramByName(name)->id);
	}
}

void ProgramManager::logShaderCompiler(const GLuint shader) const
{
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

	// Delete to avoid leak
	glDeleteShader(shader);

	LOG("SHADER COMPILER LOG START");
	LOG(&errorLog[0]);
	LOG("SHADER COMPILER LOG END");
}

void ProgramManager::logProgramLinker(const ShaderProgram* program) const
{
	GLint maxLength = 0;
	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> infoLog(maxLength);
	glGetProgramInfoLog(program->id, maxLength, &maxLength, &infoLog[0]);

	// Delete to avoid leak
	for (GLuint shader : program->shaders)
		glDeleteShader(shader);

	glDeleteProgram(program->id);

	LOG("PROGRAM LINKER LOG START");
	LOG(&infoLog[0]);
	LOG("PROGRAM LINKER LOG END");
}

void ProgramManager::compileAndAttachProgramShaders(const ShaderProgram* program) const
{
	for (GLuint shader : program->shaders) {
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			logShaderCompiler(shader);

			glAttachShader(program->id, shader);
		}
	}
}
