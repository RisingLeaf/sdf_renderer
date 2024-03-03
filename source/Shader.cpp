#include "Shader.h"
#include "Log.h"

#include <string>



Shader::Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename, const char *geometryShaderFilename)
{
	Log::Status("SHADER: init");
	shaderId = createShader(
		vertexShaderFilename, 
		fragmentShaderFilename, 
		geometryShaderFilename
	);
}

Shader::~Shader()
{
	if(shaderId > 0)
	{
		glDeleteProgram(shaderId);
	}
}

void Shader::bind()
{
	glUseProgram(shaderId);
}

void Shader::unbind()
{
	glUseProgram(0);
}

GLuint Shader::compile(std::string shaderSource, GLenum type)
{
	GLuint id = glCreateShader(type);
	const char *src = shaderSource.c_str();

	glShaderSource(id, 1, (const char **)&src, 0);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result != GL_TRUE)
	{
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		Log::Error("SHADER: compilation error:" + std::string(message));
		delete[] message;
		return 0;
	}
	return id;
}

std::string Shader::parse(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if(file == nullptr)
	{
		Log::Error("SHADER: File " + std::string(filename) + " not found");
		return "";
	}
	std::string contents;
	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	rewind(file);
	contents.resize(filesize);

	fread(&contents[0], 1, filesize, file);
	fclose(file);
	return contents;
}

GLuint Shader::createShader(const char *vertexShaderFilename, const char *fragmentShaderFilename, const char *geometryShaderFilename)
{
	std::string vertexShaderSource = parse(vertexShaderFilename);
	std::string geometryShaderSource = geometryShaderFilename ? parse(geometryShaderFilename) : "";
	std::string fragmentShaderSource = parse(fragmentShaderFilename);

	if(vertexShaderSource.empty() || fragmentShaderSource.empty() || (geometryShaderSource.empty() && geometryShaderFilename))
		Log::Error("SHADER: Empty file given!");
	Log::Status("SHADER: shaders loaded");

	GLuint program = glCreateProgram();
	GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint gs = geometryShaderFilename ? compile(geometryShaderSource, GL_GEOMETRY_SHADER) : 777;
	GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);
	Log::Status("SHADER: shaders compiled  vsh=" + std::to_string(vs) + "  fsh=" + std::to_string(fs) + "  gsh=" + std::to_string(gs));

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	if(geometryShaderFilename != nullptr)
		glAttachShader(program, gs);

	glLinkProgram(program);


	return program;
}

void Shader::LoadUniformLocation(const std::string &name)
{
	uniformLocations[name] = glGetUniformLocation(shaderId, name.c_str());
}

// COMPUTE

ComputeShader::ComputeShader( const char *computeShaderFilename )
{
	static bool testedLimits = false;
	if ( !testedLimits )
	{
		int maxWorkGroups[3];
		int maxWorkGroupSize[3];
		int maxWorkGroupInvocations;
		for ( int idx = 0; idx < 3; idx++ )
		{
			glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &maxWorkGroups[idx] );
			glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &maxWorkGroupSize[idx] );
		}
		glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxWorkGroupInvocations );
/*
		std::cout << "Opengl Limitation:" << std::endl;
		std::cout << "Max Work Group X: " << maxWorkGroups[0] << std::endl;
		std::cout << "Max Work Group Y: " << maxWorkGroups[1] << std::endl;
		std::cout << "Max Work Group Z: " << maxWorkGroups[2] << std::endl;

		std::cout << "Max Work Group Size X: " << maxWorkGroupSize[0] << std::endl;
		std::cout << "Max Work Group Size Y: " << maxWorkGroupSize[1] << std::endl;
		std::cout << "Max Work Group Size Z: " << maxWorkGroupSize[2] << std::endl;

		std::cout << "Max Work Group Invocations: " << maxWorkGroupInvocations << std::endl;
*/
		testedLimits = true;
	}
	Log::Status("SHADER: init");
	shaderId = createShader(computeShaderFilename);

	Log::Status("SHADER: shaderid=" + std::to_string(shaderId));
}

ComputeShader::~ComputeShader()
{
	if(shaderId > 0)
	{
		glDeleteProgram(shaderId);
	}
}

void ComputeShader::bind()
{
	glUseProgram(shaderId);
}

void ComputeShader::unbind()
{
	glUseProgram(0);
}

GLuint ComputeShader::compile( std::string shaderSource, GLenum type )
{
	GLuint id = glCreateShader(type);
	const char *src = shaderSource.c_str();

	glShaderSource(id, 1, (const char **) &src, 0);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result != GL_TRUE)
	{
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		Log::Error( "SHADER: compilation error:" + std::string(message));
		delete[] message;
		return 0;
	}
	return id;
}

std::string ComputeShader::parse(const char *filename)
{
	FILE *file = fopen(filename, "rb" );
	if ( file == nullptr )
	{
		Log::Error("SHADER: File " + std::string(filename) + " not found");
		return "";
	}
	std::string contents;
	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	rewind(file);
	contents.resize(filesize);

	fread(&contents[0], 1, filesize, file);
	fclose(file);

	return contents;
}

GLuint ComputeShader::createShader(const char *computeShaderFilename)
{
	std::string computeShaderSource = parse(computeShaderFilename);

	GLuint program = glCreateProgram();
	GLuint cs = compile(computeShaderSource, GL_COMPUTE_SHADER);
	Log::Status("SHADER: shaders compiled  csh=" + std::to_string(cs));

	glAttachShader(program, cs);
	glLinkProgram(program);

	glValidateProgram(program);
	
	return program;
}

void ComputeShader::loadUniformLocation(std::string name)
{
	uniformLocations[name] = glGetUniformLocation(shaderId, name.c_str());
}