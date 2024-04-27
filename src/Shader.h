#pragma once
#include <GL/glew.h>
#include <sstream>
#include <fstream>


class Shader {
	GLuint vertexShader;
	GLuint geometryShader = NULL; 
	GLuint fragmentShader;
	std::string readFile(std::string fileName);
	GLuint loadShader(GLenum shaderType, std::string fileName); 
public:
	GLuint shaderProgram;


	Shader(std::string vPath, std::string gPath, std::string fPath);
	~Shader();
	void enable();
	GLuint u(std::string vName);
	GLuint a(std::string vName);
	void setFloat(const GLchar* a , float num);
};