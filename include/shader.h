#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
 public:
  // the program ID
  unsigned int ID;

  // constructor
  Shader(const char* vertexPath, const char* fragmentPath)
  {
    // 1. retrieve the vertex/fragement source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
      {
	// open files
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	// read file's buffer contents into streams
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	// close file handlers
	vShaderFile.close();
	fShaderFile.close();
	// convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
      }
    catch(std::ifstream::failure e)
      {
	std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"
		  << std::endl;
      }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
      {
	glGetShaderInfoLog(vertex, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
	  infoLog << " " << vertexPath << std::endl;
      };

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
      {
	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
	  infoLog << " " << fragmentPath << std::endl;
      };
	
    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
      {
	glGetProgramInfoLog(ID, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
	  infoLog << std::endl;
      }

    // delete shaders, no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // use/activate the shader
  void use()
  {
    glUseProgram(ID);
  }
  // utility uniform functions
  void setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setVec2(const std::string &name, float x, float y) const
  {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  void setVec3(const std::string &name, glm::vec3 &value) const
  {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const
  {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  void setVec4(const std::string &name, const glm::vec4 &value) const
  {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z, float w) const
  {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  void setMat2(const std::string &name, const glm::mat2 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
		 &mat[0][0]);
  }
  void setMat3(const std::string &name, const glm::mat3 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
		       &mat[0][0]);
  }
  void setMat4(const std::string &name, const glm::mat4 &mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
		       &mat[0][0]);
  }
};

#endif
