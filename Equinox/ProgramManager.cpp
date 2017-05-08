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
	return true;
}

void ProgramManager::Load(std::string name, char* filepath, GLenum shaderType)
{
	if (programs.find(name) == programs.end())
	{
		FILE* shaderFile = fopen(filepath, "r");
		int fileSize = 0;

		char* shaderSource;

		fseek(shaderFile, 0, SEEK_END);
		fileSize = ftell(shaderFile);
		rewind(shaderFile);

		shaderSource = (char*)malloc(sizeof(char) * (fileSize + 1));
		fread(shaderSource, sizeof(char), fileSize, shaderFile);
		shaderSource[fileSize] = '\0';
		fclose(shaderFile);

		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, (const GLchar**)&shaderSource, NULL);

		glCompileShader(shader);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shader);
		glLinkProgram(shaderProgram);

		programs.insert(std::pair<std::string, GLint>(name, shaderProgram));

	}
	else
	{
		//TODO: show some kind of error because program with that name already exists.
	}
}
