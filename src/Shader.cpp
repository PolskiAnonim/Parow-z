#include "Shader.h"

std::string Shader::readFile(std::string fileName) {
	std::ifstream file("./shaders/" + fileName);
	std::ostringstream b;
	b << file.rdbuf();
	return b.str();
}

GLuint Shader::loadShader(GLenum shaderType, std::string fileName) {
	GLuint shader = glCreateShader(shaderType);

	std::string tempShaderSource = readFile(fileName);
	const GLchar* shaderSource = tempShaderSource.c_str();
	
	glShaderSource(shader, 1, &shaderSource, NULL); // Chwyæ
	glCompileShader(shader); // Kompiluj

	//delete[] shaderSource;
	// Wed³ug jakiegoœ artyku³u... nie powinno siê usuwaæ zmiennych openGL bo te i tak
	// przenoszone s¹ do pamiêci wewnêtrznych struktur... a jako te struktury trzymaj¹
	// te wartoœci jako wskaŸniki - usuniêcie shaderSource daje crasha.
	// Co ciekawe dlaczego w prezentacji to dzia³a to nie mam pomys³u 

	return shader;
}

Shader::Shader(std::string vPath, std::string gPath, std::string fPath) {
	vertexShader = loadShader(GL_VERTEX_SHADER, vPath);

	if (gPath != "")
		geometryShader = loadShader(GL_GEOMETRY_SHADER, gPath);

	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fPath);

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (gPath != "")
		glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	printf("Shader program created \n");
}

Shader::~Shader() {
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader != 0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	if (geometryShader != 0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
}

void Shader::enable() {
	glUseProgram(shaderProgram);
}

GLuint Shader::u(std::string vName) {
	return glGetUniformLocation(shaderProgram, vName.c_str());
}

GLuint Shader::a(std::string vName) {
	return glGetAttribLocation(shaderProgram, vName.c_str());
}

void Shader::setFloat(const GLchar* vName, float num) {
	glUniform1f(u(vName), num);
}
