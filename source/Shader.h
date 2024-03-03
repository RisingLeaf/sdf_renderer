#ifndef PTREE_SHADER_H
#define PTREE_SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include <GL/glew.h>



class Shader {
public:
  Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename, const char *geometryShaderFilename = nullptr);
  Shader() { }
  virtual ~Shader();

  Shader(const Shader& source) = delete;
  Shader &operator=( const Shader &s ) = delete;
  Shader &operator=( Shader &&s )
  {
    shaderId = s.shaderId;
    s.shaderId = 0;
    return *this;
  }

  void bind();
  void unbind();

  void LoadUniformLocation(const std::string &name);
  GLuint UniformLocation(const std::string &name) const 
  {
    const auto it = uniformLocations.find(name);
    return it != uniformLocations.end() ? uniformLocations.find(name)->second : -1;
  };
  unsigned ShaderId() const { return shaderId; }

private:
  
  unsigned int shaderId;
  std::map<std::string, GLuint> uniformLocations;

  GLuint compile(std::string shaderSource, GLenum type);
  std::string parse(const char* fileName);
  GLuint createShader(const char *vertexShaderFilename, const char *fragmentShaderFilename, const char *geometryShaderFilename);
};

struct ComputeShader
{
  ComputeShader( const char *computeShaderFilename );
  ComputeShader() {}
  virtual ~ComputeShader();

  ComputeShader( const Shader &source ) = delete;
  ComputeShader &operator=( const ComputeShader &s ) = delete;
  ComputeShader &operator=( ComputeShader &&s )
  {
    shaderId = s.shaderId;

    s.shaderId = 0;

    return *this;
  }

  void bind();
  void unbind();

  unsigned int shaderId;

  std::map<std::string, GLuint> uniformLocations;
  void loadUniformLocation( std::string name );

private:
  GLuint compile( std::string shaderSource, GLenum type );
  std::string parse( const char *fileName );
  GLuint createShader( const char *computeShaderFilename );
};

#endif