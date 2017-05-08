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

void ProgramManager::Load(const std::string &name, const char* filepath, const GLenum shaderType)
{
	if (programs.find(name) == programs.end())
	{
		FILE* shaderFile = fopen(filepath, "r");
		int fileSize = 0;

		char* shaderSource;

		fseek(shaderFile, 0, SEEK_END);
		fileSize = ftell(shaderFile);
		rewind(shaderFile);

		//move to new
		shaderSource = new char[fileSize + 1];
		fread(shaderSource, sizeof(char), fileSize, shaderFile);
		shaderSource[fileSize] = '\0';
		fclose(shaderFile);

		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, const_cast<const GLchar**>(&shaderSource), NULL);

		glCompileShader(shader);

		logShaderCompiler(shader);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, shader);
		glLinkProgram(shaderProgram);

		logProgramLinker(shaderProgram);

		delete[] shaderSource;

		programs.insert(std::pair<std::string, GLuint>(name, shaderProgram));

	}
	else
	{
		//TODO: show some kind of error because program with that name already exists.
	}
}

GLuint ProgramManager::GetProgramByName(const std::string &name) const
{
	auto it = programs.find(name);
	return it != programs.end() ? it->second : 0;
}

void ProgramManager::UseProgram(const std::string &name) const
{
	glUseProgram(GetProgramByName(name));
}

void ProgramManager::logShaderCompiler(const GLuint shader) const
{
	int log_size;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);

	char* compiler_log = new char[log_size + 1];
	int logger_size;
	glGetShaderInfoLog(shader, log_size + 1, &logger_size, reinterpret_cast<GLchar*>(&compiler_log));

	compiler_log[logger_size] = '\0';

	LOG("SHADER COMPILER LOG START");
	LOG(compiler_log);
	LOG("SHADER COMPILER LOG END");
}

void ProgramManager::logProgramLinker(const GLuint program) const
{
	int log_size;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);

	char* compiler_log = new char[log_size + 1];
	int logger_size;
	glGetProgramInfoLog(program, log_size + 1, &logger_size, reinterpret_cast<GLchar*>(&compiler_log));

	compiler_log[logger_size] = '\0';

	LOG("PROGRAM LINKER LOG START");
	LOG(compiler_log);
	LOG("PROGRAM LINKER LOG END");
}
